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
// �ε��Ѵ�.
// �̰��� �����ٸ� ������ ���������� ��� �����͸� �о �� �־�� �Ѵ�.
//
// Parsing�� ����:
// �׻� ������ �������� parsing�̶�� ���� �����Ͱ� ���� �԰ݿ� �°� �� �ִٴ� ���� ������ �Ѵ�.
// ������, ���� ���ο� ������ �ٲ�� �� �ִ°��� �����ϴٴ��� �ϴ� ��Ģ�� �����
// �˻��ϴ� ��ƾ�� ���������� ����. �ϴ��� ������� ���� �Ծ����� �� �ִٴ°��� ��������.
// -> �׷���, ��� ȣ���� �ϴ� �ϳ��� ū �Լ��� �ذ�ô�.
// -> depth�� �������� �����ۿ� ���� ���� �ڵ带 �־�߰ڴ�
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	/// 0) ������ �ε��Ѵ�.
	if (!m_lexer->Open(p_File))
	{
		TRACE("������ ���� �߿� ������ �߻��߽��ϴ�!");
		return FALSE;
	}

	/// 1) �ٷ��̾ƺ����豳���� �Ŀ�Ǯ�� ����Լ� �Ѱ��� ó�� ��!
	Parsing_DivergeRecursiveALL(0);

	return TRUE;
}

/// 1�� ��ȯ

// CScenedata ����
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// �� ����
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// �޽��� ����ȭ�� �� �ش�.
/// �븻��, �ؽ��� ��ǥ�� ���� ���ؽ��� �ø���, ��ø�Ǵ°��� �����ϰ�..
/// </summary>
bool CASEParser::Convert_Optimize(Mesh* pMesh)
{
	// 2021.04.12
	// ���� ���� (���ÿ�)

	return FALSE;
}

bool CASEParser::ConvertAll(Mesh* pMesh)
{
	// ���ؽ����� �ϴ� ��� ����
	//for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
	//{
	//	Vertex* _pVertex = new Vertex;
	//	_pVertex->m_pos = pMesh->m_meshvertex[i]->m_pos;
	//	_pVertex->m_normal = pMesh->m_meshvertex[i]->m_normal;
	//	_pVertex->u = pMesh->m_meshvertex[i]->u;
	//	_pVertex->v = pMesh->m_meshvertex[i]->v;

	//	pMesh->m_opt_vertex.push_back(_pVertex);
	//}

	// �ε����� �׳� ����
	pMesh->m_opt_index = new IndexList[pMesh->m_meshface.size()];

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[j];
		}
	}

	return FALSE;
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
// ��� ȣ����� ������ �ϴ� �б� �Լ��̴�.
//
// �ϴ� �а�, ��ū�� ���� �����Ѵ�.
//
// ��͸� ���� ������ { �� ���� ����̴�.
// �����ϴ� ������ '}'�� �����ų� TOKEND_END�� ������ ���̴�.
//
// ���Ҿ�, ��͸� ���� �� ��ū�� �о�, ���� �� �Լ��� � ��������� �����Ѵ�.
//
//----------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// ���������� ����
	// ��� �����ٰ� ���� �̻� ������ ���� ���(����)�� üũ�ϱ� ���� ����
	int i = 0;
	// ����Ʈ�� �����͸� �־�� �� �� ���̴� ����Ʈ ī����.
	int listcount = 0;		// �ſ� C����� �ٸ� �ذ�å�� ������?
	// �̰��� �� ��ū�� � �����ΰ��� �Ǻ����ش�.
	// �̰��� ��� ������ �ϰ� ��͸� �����°��� �����ϴ�? -> �߸��� ��͹���̴�?
	LONG nowtoken;
	//----------------------------------------------------------------------

	Matrix TM1, TM2;
	Quaternion tempQT;
	Quaternion prevQT;
	Quaternion resultQT;

	Face* optFace = nullptr;

	/// �̰��� �ϸ� �� ���� ��ū�� �а�, �� ������ ������ �� �� �ִ�.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// �ϴ� �� ���� ��ū�� �а�, �װ��� ��ȣ �ݱⰡ �ƴ϶��.
		// �Ѿ�� ��ū�� ���� ó�����ش�.
		static int iv = 0;
		switch (nowtoken)
		{
		case TOKEND_BLOCK_START:

			Parsing_DivergeRecursiveALL(depth++);
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
			Parsing_String();	// �׳� m_TokenString�� �о������ ���� ��.
			//AfxMessageBox( m_TokenString, NULL, NULL);		/// �ӽ÷� �ڸ�Ʈ�� ����غ���
			break;

			//--------------------
			// SCENE
			//--------------------

		case TOKENR_SCENE:
			//
			break;
		case TOKENR_SCENE_FILENAME:
			m_scenedata.m_filename = Parsing_String();		// �ϰ��� �ִ� �Լ��� ����� ���� String�� Int�� ��������.
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
			// �׳� ���� ������ �о������ }�� ���ö�����
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
			//	�� ���� �׷� ����. �� ������ �̸��� ��Ʈ������ ������� �ϴµ�.
			break;

		case TOKENR_HELPEROBJECT:
			m_parsingmode = eHelperObject;
			Create_onemesh_to_list();
			m_OneMesh->m_IsHelper = true;
			// �ϴ� �����ϰ�
			// ������Ʈ�� Ÿ�� ������. �̰Ϳ� ���� ���� �ٸ� �Ľ� ��� �ߵ�.
			break;

		case TOKENR_GEOMOBJECT:
			/// new Mesh
			m_parsingmode = eGeomobject;
			Create_onemesh_to_list();

			break;

		case TOKENR_NODE_NAME:
			// ��� ������ ������Ʈ���� ���� �� �� �ִ� ������ ���̴�.
			// ��忡 ���� �־�� �� ���� �ٸ���.
			//m_OneMesh->m_nodename = Parsing_String();
		{
			if(m_parsingmode == eHelperObject)
				m_OneMesh->m_nodename = Parsing_String();
			if (m_parsingmode == eGeomobject)
				m_OneMesh->m_nodename = Parsing_String();
			if (m_parsingmode == eAnimation)
				m_animation->m_nodename = Parsing_String();

		}
			break;

		case TOKENR_NODE_PARENT:
			// �� ����� �θ� ����� ����.
			// �ϴ� �Է��� �ϰ�, ���߿� ��������.
		{
			if (m_parsingmode == eHelperObject)
				m_OneMesh->m_nodeparent = Parsing_String();
			if (m_parsingmode == eGeomobject)
				m_OneMesh->m_nodeparent = Parsing_String();

		}
			break;

			/// NODE_TM

		case TOKENR_NODE_TM:
			//m_parsingmode	=	eGeomobject;
			// (NODE_TM���� ���� �� NODE_NAME�� �ѹ� �� ���´�.)
			// (Animation���� ������ �ؾ� �ϱ� ������ �̷��� ��带 �����ش�)


			/// �Դٰ�,
			// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
			// �ƿ� �� ��Ϳ��� ������ ��Ű�� ������. �߰��� �д°��� �����ؾ� �ϱ� ������...
			//if (m_onemesh->m_camera_isloadTarget) {
			//	'}'�� ���ö����� �����°� ������ ����! �� �̷��� �ȵǳ�..
			// ��ͱ����� �������̴�....

			break;

		case TOKENR_INHERIT_POS:
		{
			if (m_parsingmode == eHelperObject)
				m_OneMesh->m_inherit_pos = Parsing_NumberVector3();
			if (m_parsingmode == eGeomobject)
				m_OneMesh->m_inherit_pos = Parsing_NumberVector3();
			// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
		}
			break;
		case TOKENR_INHERIT_ROT:
		{
			if (m_parsingmode == eGeomobject)
				m_OneMesh->m_inherit_rot = Parsing_NumberVector3();
			if (m_parsingmode == eHelperObject)
				m_OneMesh->m_inherit_rot = Parsing_NumberVector3();
		}
			break;
		case TOKENR_INHERIT_SCL:
		{
			if (m_parsingmode == eGeomobject)
				m_OneMesh->m_inherit_scl = Parsing_NumberVector3();
			if (m_parsingmode == eHelperObject)
				m_OneMesh->m_inherit_scl = Parsing_NumberVector3();
		}
			break;
		case TOKENR_TM_ROW0:
		{
			if (m_parsingmode == eHelperObject)
			{
				m_OneMesh->m_tm_row0.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row0.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row0.y = Parsing_NumberFloat();
			}
			if (m_parsingmode == eGeomobject)
			{
				m_OneMesh->m_tm_row0.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row0.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row0.y = Parsing_NumberFloat();
			}
		}
			break;
		case TOKENR_TM_ROW1:
		{
			if (m_parsingmode == eGeomobject)
			{
				m_OneMesh->m_tm_row1.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row1.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row1.y = Parsing_NumberFloat();
			}
			if (m_parsingmode == eHelperObject)
			{
				m_OneMesh->m_tm_row1.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row1.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row1.y = Parsing_NumberFloat();
			}
		}

			break;
		case TOKENR_TM_ROW2:
			/// row 2 �� row 3 �� ���� ���� �ٲ����Ѵ� (��ǥ��) 
		{
			if (m_parsingmode == eGeomobject)
			{
				m_OneMesh->m_tm_row2.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row2.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row2.y = Parsing_NumberFloat();
			}
			if (m_parsingmode == eHelperObject)
			{
				m_OneMesh->m_tm_row2.x = Parsing_NumberFloat();
				m_OneMesh->m_tm_row2.z = Parsing_NumberFloat();
				m_OneMesh->m_tm_row2.y = Parsing_NumberFloat();
			}
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

			// ���� ī�޶� ���¿��ٸ� �̹� ��带 ���� ������ ǥ�����ش�.
			break;


			/// MESH

		case TOKENR_MESH:
		{
			/// �Ʒ��� �Լ����� m_OneMesh�� �����.

		}
		break;

		case TOKENR_TIMEVALUE:
		{
			///int _timeValue = m_lexer->GetToken(m_TokenString);
			int _timeValue = Parsing_NumberInt();
			m_OneMesh->m_timevalue = _timeValue;
		}
		break;

		case TOKENR_MESH_NUMBONE:
			// �̰� �ִٸ� �̰��� Skinned Mesh��� ������ ���´�.
			// ���� �Է�
			break;
		case TOKENR_MESH_NUMSKINWEIGHT:
			break;
		case TOKENR_MESH_NUMVERTEX:
		{
			int num = Parsing_NumberInt();
			m_OneMesh->m_mesh_numvertex = num;
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
			// ���ؽ��� ������ ����־�� �ϴµ�
			// �̹� ���ͷ� ������ �� �����Ƿ� �׳� ������ �ȴ�.
			break;
		case TOKENR_MESH_VERTEX:
			// ������ �Է�
		{
			int num = Parsing_NumberInt();
			Vertex* ver = new Vertex();
			ver->m_pos.x = Parsing_NumberFloat();
			ver->m_pos.z = Parsing_NumberFloat();
			ver->m_pos.y = Parsing_NumberFloat();

			//ver->m_pos = Parsing_NumberVector3();
			
			m_OneMesh->m_meshvertex.push_back(ver);
		}
		break;

		/// Bone

		case TOKENR_SKIN_INITTM:
			break;
		case TOKENR_BONE_LIST:
			break;
		case TOKENR_BONE:
		{
			/// ��� ü���� �� �ְ�, Bone�� �����ϰ� �ִٴ� ���� �̰��� ��Ű�� ������Ʈ��� ���̴�.
			// �� �ϳ��� ���� �ӽ� ������ ����, ���Ϳ� �ְ�
			// Bone�� �ѹ��� �о� ����
		}
		break;
		//�� ������ ���� �̸��� �־�� �Ѵ�. ������ {�� �� �� �� �������Ƿ� �ӽ� ������ �����μ� �����ؾ߰���.
		case TOKENR_BONE_NAME:
		case TOKENR_BONE_PROPERTY:
			// �� ���� ABSOLUTE�� ������� �ϴµ�, �� ���� ����.
			break;
			// �������� TM_ROW0~3�� �����µ� ���� ���õ�..

		case TOKENR_MESH_WVERTEXS:
		{

		}
		break;

		case TOKENR_MESH_WEIGHT:
		{
			// ���ؽ� �ϳ��� ������ ���� ����Ʈ�� ����
		}
		break;
		case TOKENR_BONE_BLENGING_WEIGHT:
		{
			// ��ü �� �ܰ踦 ���°ž�...
			// ����ġ �Ѱ��� ���� ����Ʈ�� �ִ´�
			/// �� ��....
		}
		break;


		/// MESH_FACE_LIST
		case TOKENR_MESH_FACE_LIST:
			//
			break;
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
			break;

			/// MESH_TVERTLIST

		case TOKENR_MESH_TVERTLIST:
			//
			break;
		case TOKENR_MESH_TVERT:
		{
			//int num = Parsing_NumberInt();
			//COneTVertex* ver = new COneTVertex();

			//ver->m_u = Parsing_NumberFloat();
			//ver->m_v = Parsing_NumberFloat();
			//ver->m_w = Parsing_NumberFloat();
			//
			//m_OneMesh->m_mesh_tvertex.push_back(ver);

			// ���ؽ��� �ε����� �����µ� ������ ������ �����Ƿ� ������.
			// ���ο� TVertex�� ���� ���Ϳ� �ִ´�
		}
		break;
		case TOKENR_MESH_NUMTVFACES:
			break;
		case TOKENR_MESH_FACENORMAL:
		{
			//iv = Parsing_NumberInt();
			//m_OneMesh->m_meshface[iv]->m_normal = Parsing_NumberVector3();
			Parsing_NumberInt();

			optFace = new Face;
			optFace->m_normal = Parsing_NumberVector3();
		}
		break;

		case TOKENR_MESH_VERTEXNORMAL:
		{
			Vertex* vertex = new Vertex;
			vertex->m_pos = (m_OneMesh->m_meshvertex[Parsing_NumberInt()])->m_pos;
			vertex->m_normal = Parsing_NumberVector3();
			vertex->m_pos = XMVector3Transform(vertex->m_pos, m_OneMesh->m_WorldTM.Invert());
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
			animscl->m_scalerotQT_accumulation.x = Parsing_NumberFloat();
			animscl->m_scalerotQT_accumulation.y = Parsing_NumberFloat();
			animscl->m_scalerotQT_accumulation.z = Parsing_NumberFloat();
			animscl->m_scalerotQT_accumulation.w = Parsing_NumberFloat();

			m_animation->m_scale.push_back(animscl);
		}
		break;

		case TOKENR_CONTROL_ROT_SAMPLE:
		{
			CAnimation_rot* animrot = new CAnimation_rot;
			animrot->m_time = Parsing_NumberLong();
			animrot->m_rotQT_accumulation.x = Parsing_NumberFloat();
			animrot->m_rotQT_accumulation.y = Parsing_NumberFloat();
			animrot->m_rotQT_accumulation.z = Parsing_NumberFloat();
			animrot->m_rotQT_accumulation.w = Parsing_NumberFloat();

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
			// �Ƹ��� �̰� ������ ���� ��Ÿ�������ΰ� ������. while�� Ż���ؾ� �ϴµ�?

			//AfxMessageBox("������ ���� �� �� �����ϴ�!", MB_OK, NULL);
			TRACE("TRACE: �Ľ���: ������ ���� �ý��ϴ�!\n");
			return;

			break;


			/// ���� �ƹ��͵� �ش����� ������
		default:
			// �ƹ��͵� ���� �ʴ´�.
			break;

		}

		///-----------------------------------------------
		/// ���� �ڵ�.
		i++;
		if (i > 1000000)
		{
			// ������ 1000000���̻��̳� �� ������ ����. (�����Ͱ� 100000���� �ƴ��̻�)
			// ���� 1000000�̻� ���Ҵٸ� Ȯ���� ���� ������ �ִ� ���̹Ƿ�
			TRACE("������ �鸸�� ���ҽ��ϴ�!");
			return;
		}
		/// ���� �ڵ�.
		///-----------------------------------------------

	}		// while()

	// ������� �Դٸ� while()�� ����ٴ� ���̰� �� ����
	// ��ȣ�� �ݾҴٴ� ���̹Ƿ�
	// �������� (����Լ�)
	auto a = m_MeshList;
	return;
}


///----------------------------------------------------------------------
/// parsing�� ���� ������ �Լ���
///----------------------------------------------------------------------

// long�� �о �������ش�.
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
	/// ��m_TokenString ( char[255] ) �̱� ������ CString�� ������ ���� ���Ŷ� �����ߴµ�, �������� CString�� �� ������� �� ����. �˾Ƽ� �޾Ƶ��̴µ�?
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


// 3���� Float�� ���� �ϳ���
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

	return			tempVector3;		// ����ƽ ������ ���۷������ٴ� �� ������ ����.
}

///--------------------------------------------------
/// ���ο��� ������ ����, ����Ʈ�� �ִ´�
///--------------------------------------------------
// �޽ø� �ϳ� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_onemesh_to_list()
{
	Mesh* temp = new Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// Ŭ������ �� ����
	m_MeshList.push_back(m_OneMesh);
}

// ��Ʈ���� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_materialdata_to_list()
{
	ASEMaterial* temp = new ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

// �ִϸ��̼ǵ����� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_animationdata_to_list()
{
	Animation* temp = new Animation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}

// ���� �ϳ���..
void CASEParser::Create_onevertex_to_list()
{
	Vertex* temp = new Vertex;
	m_OneMesh->m_meshvertex.push_back(temp);
}

















