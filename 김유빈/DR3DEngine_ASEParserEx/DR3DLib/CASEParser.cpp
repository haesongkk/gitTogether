#include "CASEParser.h"


CASEParser::CASEParser()
{
	m_materialcount = 0;
	m_parsingmode = eNone;
}

CASEParser::~CASEParser()
{
	delete m_lexer;
}

bool CASEParser::Init()
{
	m_lexer = new ASE::CASELexer;

	return TRUE;
}
//---------------------------------------------------------------------------------------------------
// 로딩한다.
// 이것이 끝났다면 정해진 데이터형에 모든 데이터를 읽어서 들어가 있어야 한다.
//
// Parsing에 대해:
// 항상 느끼는 것이지만 parsing이라는 것은 데이터가 일정 규격에 맞게 들어가 있다는 것을 전제로 한다.
// 하지만, 파일 내부에 순서가 바뀌어 들어가 있는것이 가능하다던지 하는 규칙이 생기면
// 검색하는 루틴이 복잡해지기 마련. 일단은 순서대로 일정 규약으로 들어가 있다는것을 가정하자.
// -> 그래서, 재귀 호출을 하는 하나의 큰 함수로 해결봤다.
// -> depth를 기준으로 오동작에 대한 안전 코드를 넣어야겠다
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	/// 0) 파일을 로드한다.
	if (!m_lexer->Open(p_File))
	{
		TRACE("파일을 여는 중에 문제가 발생했습니다!");
		return FALSE;
	}

	/// 1) 다롱이아부지김교수의 파워풀한 재귀함수 한개로 처리 끝!
	Parsing_DivergeRecursiveALL(0);

	return TRUE;
}

/// 1차 변환

// CScenedata 복사
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// 값 복사
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// 메시의 최적화를 해 준다.
/// 노말값, 텍스쳐 좌표에 따라 버텍스를 늘리고, 중첩되는것은 제거하고..
/// </summary>
bool CASEParser::Convert_Optimize(Mesh* pMesh)
{
	// 2021.04.12
	// 구현 제거 (예시용)

	return FALSE;
}

bool CASEParser::ConvertAll(Mesh* pMesh)
{
	// 버텍스들은 일단 모두 복사
	//for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
	//{
	//	Vertex* _pVertex = new Vertex;
	//	_pVertex->m_pos = pMesh->m_meshvertex[i]->m_pos;
	//	_pVertex->m_normal = pMesh->m_meshvertex[i]->m_normal;
	//	_pVertex->u = pMesh->m_meshvertex[i]->u;
	//	_pVertex->v = pMesh->m_meshvertex[i]->v;

	//	pMesh->m_opt_vertex.push_back(_pVertex);
	//}

	// 인덱스는 그냥 복사
	pMesh->m_opt_index = new IndexList[pMesh->m_meshface.size()];

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[j];
		}
	}

	for (auto& a : m_list_animation)
	{
		if (a->m_nodename == pMesh->m_nodename)
			pMesh->m_Animation = *a;
	}
	
	for (auto bone : pMesh->m_vector_bone_list)
	{
		for (int i = 0; i < m_MeshList.size(); i++)
		{
			if (bone->m_bone_name == m_MeshList[i]->m_nodename)
			{
				pMesh->m_vector_boneMesh_list.push_back(m_MeshList[i]);
			}
		}
	}

	for (int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		int keyToFind = i;

		auto lower = pMesh->m_NonSplitVertexIndex.lower_bound(keyToFind);
		auto upper = pMesh->m_NonSplitVertexIndex.upper_bound(keyToFind);

		// keyToFind에 해당하는 모든 값을 출력
		for (auto j = lower; j != upper; j++) 
		{
			j->second->m_boneIndexNum[0] = pMesh->m_meshvertex[i]->m_boneIndexNum[0];
			j->second->m_boneIndexNum[1] = pMesh->m_meshvertex[i]->m_boneIndexNum[1];
			j->second->m_boneIndexNum[2] = pMesh->m_meshvertex[i]->m_boneIndexNum[2];
			j->second->m_boneIndexNum[3] = pMesh->m_meshvertex[i]->m_boneIndexNum[3];

			j->second->m_bw[0] = pMesh->m_meshvertex[i]->m_bw[0];
			j->second->m_bw[1] = pMesh->m_meshvertex[i]->m_bw[1];
			j->second->m_bw[2] = pMesh->m_meshvertex[i]->m_bw[2];
			j->second->m_bw[3] = pMesh->m_meshvertex[i]->m_bw[3];
		}
	}

	pMesh->m_NonSplitVertexIndex.clear();

	//float determinant = DirectX::XMVectorGetX(DirectX::XMMatrixDeterminant(pMesh->m_WorldTM));

	//if (determinant < 0.f)
	//{
	//	pMesh->m_IsNegative = true;
	//	// 엄마가 있는데 네거티브
	//	if (!pMesh->m_nodeparent.empty())
	//	{
	//		// 자기가 네거티브인데 엄마가 네거티브가 아니라면 자기가 네거티브의 시작이 된다
	//		for (auto parent : m_MeshList)
	//		{
	//			if (parent->m_nodename == pMesh->m_nodeparent)
	//			{
	//				if (parent->m_IsNegative == false)
	//					pMesh->m_IsNegativeRoot = true;
	//			}
	//		}
	//	}
	//	// 엄마가 없는데 네거티브 -> 걍 자기가 네거티브 루트
	//	else
	//	{
	//		pMesh->m_IsNegativeRoot = true;
	//	}
	//}
	//else
	//{
	//	// 엄마가 있는데 네거티브가 아님 -> 엄마도 자기도 네거티브라 그런것인지 확인한다
	//	if (!pMesh->m_nodeparent.empty())
	//	{
	//		for (auto parent : m_MeshList)
	//		{
	//			if (parent->m_nodename == pMesh->m_nodeparent)
	//			{
	//				if (parent->m_IsNegative == true)
	//					pMesh->m_IsNegativeNotRoot = true;
	//			}
	//		}
	//	}
	//}

	if (pMesh->m_nodeparent.empty())
	{
		pMesh->m_LocalTM = pMesh->m_WorldTM;
	}
	else
	{
		for (auto& m : m_MeshList)
		{
			if (pMesh->m_nodeparent == m->m_nodename)
			{
				pMesh->m_LocalTM = pMesh->m_WorldTM * m->m_WorldTM.Invert();
			}
		}
	}

	//Vector3 posW, scaleW;
	//Quaternion rotW;

	//pMesh->m_LocalTM.Decompose(scaleW, rotW, posW);

	//if (pMesh->m_type == eObjectType::eObjectType_Shape
	//	&& pMesh->m_isAnimated == false)
	//{
	//	if (pMesh->m_IsNegativeRoot
	//		|| pMesh->m_IsNegativeNotRoot)
	//	{
	//		pMesh->m_LocalTM = Matrix::CreateScale({ 1.f, -1.f, 1.f })
	//			* Matrix::CreateFromQuaternion(rotW)
	//			* Matrix::CreateTranslation(posW);
	//	}
	//}
		

	return FALSE;
}

void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// 지역변수들 선언
	// 재귀 돌리다가 일정 이상 루프를 도는 경우(오류)를 체크하기 위한 변수
	int i = 0;
	// 리스트에 데이터를 넣어야 할 때 쓰이는 리스트 카운터.
	int listcount = 0;		// 매우 C스러운데 다른 해결책이 없을까?
	// 이것은 현 토큰이 어떤 종류인가를 판별해준다.
	// 이것을 멤버 변수로 하고 재귀를 돌리는것은 위험하다? -> 잘못된 재귀방식이다?
	LONG nowtoken;
	//----------------------------------------------------------------------

	Matrix TM1, TM2;
	Quaternion tempQT;
	Quaternion prevQT;
	Quaternion resultQT;

	Face* optFace = nullptr;

	/// 이것을 하면 한 개의 토큰을 읽고, 그 종류와 내용을 알 수 있다.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// 일단 한 개의 토큰을 읽고, 그것이 괄호 닫기가 아니라면.
		// 넘어온 토큰에 따라 처리해준다.
		static int iv = 0;
		static int verIndex = 0;
		
		switch (nowtoken)
		{
		case TOKEND_BLOCK_START:

			Parsing_DivergeRecursiveALL(depth++);
			break;

			/// Skinning
		case TOKENR_BONE_LIST:
			//m_parsingmode = eBone_List;
			break;

		case TOKENR_MESH_NUMBONE:
		{
			m_OneMesh->m_numbone = Parsing_NumberInt();
		}
		break;
		
		case TOKENR_BONE:
		{
			Bone* bone = new Bone;
			bone->m_bone_number = Parsing_NumberInt();
			m_OneMesh->m_vector_bone_list.push_back(bone);
		}
		break;

		case TOKENR_BONE_NAME:
		{
			m_OneMesh->m_vector_bone_list.back()->m_bone_name = Parsing_String();
		}
		break;

		case TOKENR_MESH_WEIGHT:
		{
			//m_OneMesh->m_is_skinningobject = true;

			VertexWeight* vWeight = new VertexWeight;
			// 가중치를 반영시킬 정점의 인덱스
			vWeight->m_wvertex_number = Parsing_NumberInt();
			// m_wvertex 는 임시변수이다.
			m_OneMesh->m_vector_wvertexs.push_back(vWeight);
		}
		break;

		case TOKENR_BONE_BLENGING_WEIGHT:
		{
			Weight* w = new Weight;
			// 가중치를 반영할 본의 인덱스
			w->m_bone_number = Parsing_NumberInt();
			w->m_bone_weight = Parsing_NumberFloat();
			m_OneMesh->m_vector_wvertexs.back()->m_bone_blending_weight.push_back(w);

			// 가중치가 여럿일때 순차적으로 넣어주기 위한 인덱스
			VertexWeight* vWeight = m_OneMesh->m_vector_wvertexs.back();
			int weightIndex = vWeight->m_bone_blending_weight.size();
			int boneNum = w->m_bone_number;
			float weight = w->m_bone_weight;

			m_OneMesh->m_meshvertex[vWeight->m_wvertex_number]->m_boneIndexNum[weightIndex - 1] = boneNum;
			m_OneMesh->m_meshvertex[vWeight->m_wvertex_number]->m_bw[weightIndex - 1] = weight;

			int a = 1;
			//m_OneMesh->m_opt_vertex[m_OneMesh->m_wvertex->m_wvertex_number]->m_boneIndexNum[weightIndex - 1] = boneNum;
			//m_OneMesh->m_opt_vertex[m_OneMesh->m_wvertex->m_wvertex_number]->m_bw[weightIndex - 1] = weight;
		}
		break;

		case TOKENR_HELPER_CLASS:
			m_parsingmode = eHelperObject;
			break;

			//--------------------
			// 3DSMAX_ASCIIEXPORT
			//--------------------

		case TOKENR_3DSMAX_ASCIIEXPORT:
			m_data_asciiexport = Parsing_NumberLong();
			break;

			//--------------------
			// COMMENT
			//--------------------

		case TOKENR_COMMENT:
			Parsing_String();	// 그냥 m_TokenString에 읽어버리는 역할 뿐.
			//AfxMessageBox( m_TokenString, NULL, NULL);		/// 임시로 코멘트를 출력해본다
			break;

			//--------------------
			// SCENE
			//--------------------

		case TOKENR_SCENE:
			//
			break;
		case TOKENR_SCENE_FILENAME:
			m_scenedata.m_filename = Parsing_String();		// 일관성 있는 함수의 사용을 위해 String과 Int도 만들어줬다.
			break;
		case TOKENR_SCENE_FIRSTFRAME:
			m_scenedata.m_firstframe = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_LASTFRAME:
			m_scenedata.m_lastframe = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_FRAMESPEED:
			m_scenedata.m_framespeed = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_TICKSPERFRAME:
			m_scenedata.m_ticksperframe = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_MESHFRAMESTEP:
			m_scenedata.m_meshframestep = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_KEYFRAMESTEP:
			m_scenedata.m_keyframestep = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_BACKGROUND_STATIC:
			m_scenedata.m_scene_background_static.x = Parsing_NumberFloat();
			m_scenedata.m_scene_background_static.y = Parsing_NumberFloat();
			m_scenedata.m_scene_background_static.z = Parsing_NumberFloat();
			break;
		case TOKENR_SCENE_AMBIENT_STATIC:
			m_scenedata.m_scene_ambient_static.x = Parsing_NumberFloat();
			m_scenedata.m_scene_ambient_static.y = Parsing_NumberFloat();
			m_scenedata.m_scene_ambient_static.z = Parsing_NumberFloat();
			break;

		case TOKENR_SCENE_ENVMAP:
		{
			// 그냥 안의 내용을 읽어버린다 }가 나올때까지
			while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END) {
			}
		}
		break;

		//--------------------
		// MATERIAL_LIST
		//--------------------


		//--------------------
		// GEOMOBJECT
		//--------------------

		case TOKENR_GROUP:
			//	한 개의 그룹 시작. 이 다음에 이름이 스트링으로 나오기는 하는데.
			break;

		case TOKENR_HELPEROBJECT:
		{
			m_parsingmode = eHelperObject;
			Create_onemesh_to_list();
			m_OneMesh->m_IsHelper = true;
			iv = 0;
			verIndex = 0;

			// 일단 생성하고
			// 오브젝트의 타입 정해줌. 이것에 따라 서로 다른 파싱 모드 발동.
		}
		break;


		case TOKENR_GEOMOBJECT:
			/// new Mesh
		{
			m_parsingmode = eGeomobject;
			Create_onemesh_to_list();
			iv = 0;
			verIndex = 0;
		}
		break;

		case TOKENR_SHAPEOBJECT:
			/// new Mesh
			m_parsingmode = eShape;
			Create_onemesh_to_list();
			m_OneMesh->m_type = eObjectType::eObjectType_Shape;
			iv = 0;
			verIndex = 0;
			break;

		case TOKENR_NODE_NAME:
			// 어쩄든 지금은 오브젝트들을 구별 할 수 있는 유일한 값이다.
			// 모드에 따라 넣어야 할 곳이 다르다.
			//m_OneMesh->m_nodename = Parsing_String();
		{
			if (m_parsingmode == eAnimation)
			{
				m_animation->m_nodename = Parsing_String();

				for (auto m : m_MeshList)
				{
					if (m->m_nodename == m_animation->m_nodename)
						m->m_isAnimated = true;
				}
			}
			else
			{
				m_OneMesh->m_nodename = Parsing_String();
			}
		}
		break;

		case TOKENR_NODE_PARENT:
			// 현 노드의 부모 노드의 정보.
			// 일단 입력을 하고, 나중에 정리하자.
		{
			m_OneMesh->m_nodeparent = Parsing_String();
		}
		break;

		/// NODE_TM

		case TOKENR_NODE_TM:
			//m_parsingmode	=	eGeomobject;
			// (NODE_TM으로 진입 후 NODE_NAME이 한번 더 나온다.)
			// (Animation과도 구별을 해야 하기 때문에 이렇게 모드를 적어준다)


			/// 게다가,
			// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
			// 아예 이 재귀에서 리턴을 시키고 싶지만. 중간에 읽는것을 무시해야 하기 때문에...
			//if (m_onemesh->m_camera_isloadTarget) {
			//	'}'이 나올때까지 나오는건 무조건 무시! 뭐 이런거 안되나..
			// 재귀구조의 문제점이다....

			break;

		case TOKENR_INHERIT_POS:
		{
			m_OneMesh->m_inherit_pos = Parsing_NumberVector3();

			// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
		}
		break;
		case TOKENR_INHERIT_ROT:
		{
			m_OneMesh->m_inherit_rot = Parsing_NumberVector3();
		}
		break;
		case TOKENR_INHERIT_SCL:
		{
			m_OneMesh->m_inherit_scl = Parsing_NumberVector3();
		}
		break;
		case TOKENR_TM_ROW0:
		{
			m_OneMesh->m_tm_row0.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_row0.z = Parsing_NumberFloat();
			m_OneMesh->m_tm_row0.y = Parsing_NumberFloat();
		}
		break;
		case TOKENR_TM_ROW1:
		{
			m_OneMesh->m_tm_row1.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_row1.z = Parsing_NumberFloat();
			m_OneMesh->m_tm_row1.y = Parsing_NumberFloat();
		}

		break;
		case TOKENR_TM_ROW2:
			/// row 2 와 row 3 의 값이 서로 바껴야한다 (좌표계) 
		
			{
				m_OneMesh->m_tm_row2.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row2.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row2.y = Parsing_NumberFloat();
			}

			break;
		case TOKENR_TM_ROW3:
		{
			m_OneMesh->m_tm_row3.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_row3.z = Parsing_NumberFloat();
			m_OneMesh->m_tm_row3.y = Parsing_NumberFloat();

			Matrix worldTM
			{
				m_OneMesh->m_tm_row0.x,m_OneMesh->m_tm_row0.y,m_OneMesh->m_tm_row0.z, 0,
				m_OneMesh->m_tm_row2.x,m_OneMesh->m_tm_row2.y,m_OneMesh->m_tm_row2.z, 0,
				m_OneMesh->m_tm_row1.x,m_OneMesh->m_tm_row1.y,m_OneMesh->m_tm_row1.z, 0,
				m_OneMesh->m_tm_row3.x,m_OneMesh->m_tm_row3.y,m_OneMesh->m_tm_row3.z, 1
			};

			m_OneMesh->m_WorldTM = worldTM;
			m_OneMesh->m_NodeTM = worldTM;
		}
		break;
		case TOKENR_TM_POS:
			//if (m_parsingmode == eGeomobject)
			m_OneMesh->m_tm_pos = Parsing_NumberVector3();

			break;
		case TOKENR_TM_ROTAXIS:
			//if (m_parsingmode == eGeomobject)
			m_OneMesh->m_tm_rotaxis = Parsing_NumberVector3();

			break;
		case TOKENR_TM_ROTANGLE:
			//if (m_parsingmode == eGeomobject)
			m_OneMesh->m_tm_rotangle = Parsing_NumberFloat();

			break;
		case TOKENR_TM_SCALE:
			//if (m_parsingmode == eGeomobject)
			m_OneMesh->m_tm_scale = Parsing_NumberVector3();

			break;
		case TOKENR_TM_SCALEAXIS:
			//if (m_parsingmode == eGeomobject)
			m_OneMesh->m_tm_scaleaxis = Parsing_NumberVector3();

			break;
		case TOKENR_TM_SCALEAXISANG:
			//if (m_parsingmode == eGeomobject)
			m_OneMesh->m_tm_scaleaxisang = Parsing_NumberFloat();

			// 현재 카메라 상태였다면 이미 노드를 읽은 것으로 표시해준다.
			break;

		case TOKENR_SHAPE_LINECOUNT:
			m_OneMesh->m_shape_linecount = Parsing_NumberInt();
			break;

		case TOKENR_SHAPE_LINE:
		{
			ShapeLine* line = new ShapeLine;
			line->m_line_number = Parsing_NumberInt();
			m_OneMesh->m_vector_shape_line.push_back(line);

		}
		break;

		/// shape obj
		case TOKENR_SHAPE_CLOSED:
			m_OneMesh->m_vector_shape_line.back()->m_shape_closed = true;
			break;

		case TOKENR_SHAPE_VERTEXCOUNT:
			m_OneMesh->m_vector_shape_line.back()->m_shape_vertexcount = Parsing_NumberInt();
			break;

		case TOKENR_SHAPE_VERTEX_KNOT:
		{
			ShapeVertex* shapever = new ShapeVertex;
			shapever->m_indexnum = Parsing_NumberInt();
			shapever->m_isknot = true;
			shapever->m_pos = Parsing_NumberVector3();
			m_OneMesh->m_vector_shape_line.back()->m_shapevertex.push_back(shapever);
			break;
		}


		case TOKENR_SHAPE_VERTEX_INTERP:
		{
			ShapeVertex* vI = new ShapeVertex;
			vI->m_indexnum = Parsing_NumberInt();
			vI->m_isknot = false;
			vI->m_pos = Parsing_NumberVector3();

			m_OneMesh->m_vector_shape_line.back()->m_shapevertex.push_back(vI);
			break;
		}

			/// MESH

		case TOKENR_MESH:
		{
			/// 아래의 함수에서 m_OneMesh가 생긴다.

		}
		break;

		case TOKENR_TIMEVALUE:
		{
			///int _timeValue = m_lexer->GetToken(m_TokenString);
			int _timeValue = Parsing_NumberInt();
			m_OneMesh->m_timevalue = _timeValue;
		}
		break;

		case TOKENR_MESH_NUMSKINWEIGHT:
			break;
		case TOKENR_MESH_NUMVERTEX:
		{
			int num = Parsing_NumberInt();
			//m_OneMesh->m_mesh_numvertex = num;
		}
		break;
		case TOKENR_MESH_NUMFACES:
		{
			int num = Parsing_NumberInt();
			m_OneMesh->m_mesh_numfaces = num;
		}
		break;

		/// MESH_VERTEX_LIST

		case TOKENR_MESH_VERTEX_LIST:
			//
			// 버텍스의 값들을 집어넣어야 하는데
			// 이미 벡터로 선언이 돼 있으므로 그냥 넣으면 된다.
			break;
		case TOKENR_MESH_VERTEX:
			// 데이터 입력
		{
			int num = Parsing_NumberInt();
			Vertex* ver = new Vertex();
			ver->m_pos = Parsing_NumberVector3();
			m_OneMesh->m_meshvertex.push_back(ver);
		}
		break;

		/// Bone

		case TOKENR_SKIN_INITTM:
			break;


		/// MESH_FACE_LIST
		case TOKENR_MESH_FACE:
		{
			//string str = Parsing_String();
			//str = Parsing_String();

			//Face* face = new Face;
			//face->m_vertexindex[0] = Parsing_NumberInt();
			//str = Parsing_String();
			//face->m_vertexindex[1] = Parsing_NumberInt();
			//str = Parsing_String();				
			//face->m_vertexindex[2] = Parsing_NumberInt();
			//str = Parsing_String();

			//m_OneMesh->m_meshface.push_back(face);
		}
		break;


		case TOKENR_MESH_NUMTVERTEX:
			m_OneMesh->m_mesh_numtvertex = Parsing_NumberInt();
			break;

///------------------------텍스쳐용-------------------------------------

		case TOKENR_MESH_TVERTLIST:
			//
			break;
		case TOKENR_MESH_TVERT:
		{
			int num = Parsing_NumberInt();
			COneTVertex* ver = new COneTVertex();

			ver->m_u = Parsing_NumberFloat();
			ver->m_v = 1.f - Parsing_NumberFloat();

			m_OneMesh->m_mesh_tvertex.push_back(ver);

			// 버텍스의 인덱스가 나오는데 어차피 순서와 같으므로 버린다.
			// 새로운 TVertex를 만들어서 벡터에 넣는다
		}
		break;
		case TOKENR_MESH_TFACE:
		{
			Parsing_NumberInt();

			optFace = new Face;
			optFace->m_TFace[0] = Parsing_NumberInt();
			optFace->m_TFace[1] = Parsing_NumberInt();
			optFace->m_TFace[2] = Parsing_NumberInt();

			m_OneMesh->m_meshface.push_back(optFace);
		}
		break;

		case TOKENR_MESH_NUMTVFACES:
		{
			m_OneMesh->m_mesh_tvfaces = Parsing_NumberInt();
			m_OneMesh->m_mesh_numfaces = 0;
		}
		break;

///------------------------------------------------------------------------------
		case TOKENR_MESH_FACENORMAL:
		{
			if (m_OneMesh->m_mesh_tvfaces == 0)
			{
				Parsing_NumberInt();

				optFace = new Face;
				optFace->m_normal = Parsing_NumberVector3();
			}
			else
			{
				iv = Parsing_NumberInt();
				//m_OneMesh->m_meshface[iv]->m_normal = Parsing_NumberVector3();

				/// 여기서 최종적으로 다 때린다. ASE 파일 순서 상 한번에 면 하나 생성하기 딱 좋은 토큰 위치라고 생각 (했지만 결국 바꿨쥬?)
				m_OneMesh->m_meshface[iv]->m_normal = Parsing_NumberVector3();
			}
		}
		break;

		case TOKENR_MESH_VERTEXNORMAL:
		{
			if (m_OneMesh->m_mesh_tvfaces == 0)
			{
				int num = Parsing_NumberInt();

				Vertex* vertex = new Vertex;
				vertex->m_pos = (m_OneMesh->m_meshvertex[num])->m_pos;
				vertex->m_normal = Parsing_NumberVector3();
				vertex->m_pos = XMVector3Transform(vertex->m_pos, m_OneMesh->m_WorldTM.Invert());
				m_OneMesh->m_NonSplitVertexIndex.insert(make_pair(num, vertex));
				m_OneMesh->m_opt_vertex.push_back(vertex);
				m_OneMesh->m_mesh_numvertex++;

				if (iv == 3)
				{
					iv = 0;
					m_OneMesh->m_meshface.push_back(optFace);
				}

				optFace->m_vertexindex[iv] = m_OneMesh->m_opt_vertex.size() - 1;
				iv++;
			}
			else
			{
				int num = Parsing_NumberInt();

				if (verIndex == 3)											// 0,1,2 순으로 증가시키며 인덱스 값을 넣다가 3이되면 다시 0
				{
					verIndex = 0;
					m_OneMesh->m_mesh_numfaces++;
				}

				// 위에서 쪼개지 않은채 받았던 포지션과 텍스쳐 좌표 값을 받고 노말도 집어넣기

				Vertex* vertex = new Vertex;

				vertex->m_pos = (m_OneMesh->m_meshvertex[num])->m_pos;
				vertex->m_normal = Parsing_NumberVector3();

				// FACENORMAL 에서 받아온 면 번호로 해당 면을 m_meshface 에서 찾고, 거기 있는 텍스쳐 인덱스 값을 순차적으로 받아와 집어 넣는다 ㅋ 진짜 킹받네
				vertex->u = (m_OneMesh->m_mesh_tvertex[(m_OneMesh->m_meshface[iv])->m_TFace[verIndex]])->m_u;
				vertex->v = m_OneMesh->m_mesh_tvertex[(m_OneMesh->m_meshface[iv])->m_TFace[verIndex]]->m_v;
				m_OneMesh->m_NonSplitVertexIndex.insert(make_pair(num, vertex));
				m_OneMesh->m_opt_vertex.push_back(vertex);
				m_OneMesh->m_mesh_numvertex++;

				/// 인덱스  
				// 파일 순서가 면 -> 해당 면을 구성하는 점들 셋이기 때문에 크기 - 1이 곧 해당 점의 새로운 번호
				(m_OneMesh->m_meshface[iv])->m_vertexindex[verIndex] = m_OneMesh->m_opt_vertex.size() - 1;
				verIndex++;
			}
		}
		break;

		case TOKENR_TM_ANIMATION:
		{
			Create_animationdata_to_list();
			m_parsingmode = eAnimation;
		}
		break;

		case TOKENR_CONTROL_SCALE_SAMPLE:
		{
			CAnimation_scl* animscl = new CAnimation_scl;
			animscl->m_time = Parsing_NumberLong();
			animscl->m_scale = Parsing_NumberVector3();
			animscl->m_scaleaxisang = Parsing_NumberFloat();

			m_animation->m_scale.push_back(animscl);
		}
		break;

		case TOKENR_CONTROL_ROT_SAMPLE:
		{
			CAnimation_rot* animrot = new CAnimation_rot;
			animrot->m_time = Parsing_NumberLong();
			animrot->m_rot = Parsing_NumberVector3();
			animrot->m_angle = Parsing_NumberFloat();

			m_animation->m_rotation.push_back(animrot);
		}
		break;

		case TOKENR_CONTROL_POS_SAMPLE:
		{
			CAnimation_pos* animpos = new CAnimation_pos;
			animpos->m_time = Parsing_NumberLong();
			animpos->m_pos = Parsing_NumberVector3();
			m_animation->m_position.push_back(animpos);
		}
		break;

		case TOKEND_END:
			// 아마도 이건 파일의 끝이 나타났을때인것 같은데. while을 탈출해야 하는데?

			//AfxMessageBox("파일의 끝을 본 것 같습니다!", MB_OK, NULL);
			TRACE("TRACE: 파싱중: 파일의 끝을 봤습니다!\n");
			return;

			break;


			/// 위의 아무것도 해당하지 않을때
		default:
			// 아무것도 하지 않는다.
			break;

		}

		///-----------------------------------------------
		/// 안전 코드.
		i++;
		if (i > 1000000)
		{
			// 루프를 1000000번이상이나 돌 이유가 없다. (데이터가 100000개가 아닌이상)
			// 만약 1000000이상 돌았다면 확실히 뭔가 문제가 있는 것이므로
			TRACE("루프를 백만번 돌았습니다!");
			return;
		}
		/// 안전 코드.
		///-----------------------------------------------

	}	// while()

			// 여기까지 왔다면 while()을 벗어났다는 것이고 그 말은
			// 괄호를 닫았다는 것이므로
			// 리턴하자 (재귀함수)
		auto a = m_MeshList;
		return;
}

ASEParser::Mesh* CASEParser::GetMesh(int index)
{
	return m_MeshList[index];
}

int CASEParser::GetMeshNum()
{
	return m_MeshList.size();
}

list<Animation*>& CASEParser::GetAnimation()
{
	return m_list_animation;
}

//----------------------------------------------------------------
// 재귀 호출됨을 전제로 하는 분기 함수이다.
//
// 일단 읽고, 토큰에 따라 동작한다.
//
// 재귀를 들어가는 조건은 { 을 만날 경우이다.
// 리턴하는 조건은 '}'를 만나거나 TOKEND_END를 만났을 때이다.
//
// 더불어, 재귀를 들어가기 전 토큰을 읽어, 지금 이 함수가 어떤 모드인지를 결정한다.
//
//----------------------------------------------------------------



///----------------------------------------------------------------------
/// parsing을 위한 단위별 함수들
///----------------------------------------------------------------------

// long을 읽어서 리턴해준다.
int CASEParser::Parsing_NumberLong()
{
	LONG			token;
	LONG			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(m_TokenString, NULL, 10);

	return			tempNumber;
}

// float
float CASEParser::Parsing_NumberFloat()
{
	LONG			token;
	float			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(m_TokenString);

	return			tempNumber;
}

// String
LPSTR CASEParser::Parsing_String()
{
	/// ※m_TokenString ( char[255] ) 이기 때문에 CString에 넣으면 에러 날거라 생각했는데, 생각보다 CString은 잘 만들어진 것 같다. 알아서 받아들이는데?
	m_lexer->GetToken(m_TokenString);

	return m_TokenString;
}


// int
int CASEParser::Parsing_NumberInt() {

	LONG			token;
	int				tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(m_TokenString);

	return			tempNumber;
}


// 3개의 Float를 벡터 하나로
Vector3 CASEParser::Parsing_NumberVector3()
{
	LONG				token;
	Vector3			tempVector3;

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.z = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.y = (float)atof(m_TokenString);

	return			tempVector3;		// 스태틱 변수의 레퍼런스보다는 값 전달을 하자.
}

///--------------------------------------------------
/// 내부에서 뭔가를 생성, 리스트에 넣는다
///--------------------------------------------------
// 메시를 하나 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_onemesh_to_list()
{
	Mesh* temp = new Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// 클래스간 값 복사
	m_MeshList.push_back(m_OneMesh);
}

// 메트리얼 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_materialdata_to_list()
{
	ASEMaterial* temp = new ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

// 애니메이션데이터 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_animationdata_to_list()
{
	Animation* temp = new Animation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}

// 정점 하나를..
void CASEParser::Create_onevertex_to_list()
{
	Vertex* temp = new Vertex;
	m_OneMesh->m_meshvertex.push_back(temp);
}

















