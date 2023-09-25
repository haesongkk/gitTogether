#include "ObjLoader.h"
#include "Box.h"
#include <cassert>
#include <SimpleMath.h>
#include "MeshObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

using namespace std;

ObjLoader::ObjLoader(ID3D11Device* _device, ID3D11DeviceContext* _dc)
	:g_pDevice(_device), m_pDeviceContext(_dc)
{
}

void ObjLoader::Load_MTL_File(const char* path)
{
	FILE* mtl = nullptr;
	fopen_s(&mtl, path, "rb");
	assert(mtl);
	//Metarial Data 
	string key;
	Color Ka, Kd, Ks;
	Ka.w = Kd.w = Ks.w = 1.0f;
	float d;

	while (!feof(mtl))
	{
		char buffer[1024];
		fgets(buffer, 1024, mtl);

		string sBuf = buffer;

		if (sBuf.find("\n") != string::npos)
			sBuf.erase(sBuf.find("\n"), sBuf.size());

		if (sBuf.find("\r") != string::npos)
			sBuf.erase(sBuf.find("\r"), sBuf.size());


		// #		Tell Information
		if (sBuf.find("#") != string::npos) {}

		// newmtl	Mtl Call Key
		else if (sBuf.find("newmtl ") != string::npos) {
			key = sBuf;
			key.erase(0, key.find(" ") + 1);

			if (mMetarial.count(key))
				break;

			else {
				MetarialData* meta = new MetarialData;
				mMetarial.insert(make_pair(key, meta));
				meta->metarial.Ambient.w = 1.0f;
				meta->metarial.Diffuse.w = 1.0f;
				meta->metarial.Specular.w = 1.0f;
				meta->metarial.Reflect = Vector4(0, 0, 0, 0);
			}
		}

		// Ka	Ambient Value
		else if (sBuf.find("Ka  ") != string::npos) {
			string value = sBuf;
			BYTE _next = 0;
			value.erase(0, value.find_first_of(" ") + 2);		//Ka Delete

			while (_next < (BYTE)3) {
				string ka;
				ka.append(value, 0, value.find_first_of(" "));
				value.erase(0, value.find_first_of(" ") + 1);

				if (_next == (BYTE)0)
					mMetarial[key]->metarial.Ambient.x = atof(ka.c_str());

				else if (_next == (BYTE)1)
					mMetarial[key]->metarial.Ambient.y = atof(ka.c_str());

				else if (_next == (BYTE)2)
					mMetarial[key]->metarial.Ambient.z = atof(ka.c_str());

				++_next;
			}
		}

		// Kd	Diffuse Value
		else if (sBuf.find("Kd  ") != string::npos) {
			string value = sBuf;
			BYTE _next = 0;
			value.erase(0, value.find_first_of(" ") + 2);		//Kd Delete

			while (_next < (BYTE)3) {
				string kd;
				kd.append(value, 0, value.find_first_of(" "));
				value.erase(0, value.find_first_of(" ") + 1);

				if (_next == (BYTE)0)
					mMetarial[key]->metarial.Diffuse.x = atof(kd.c_str());

				else if (_next == (BYTE)1)
					mMetarial[key]->metarial.Diffuse.y = atof(kd.c_str());

				else if (_next == (BYTE)2)
					mMetarial[key]->metarial.Diffuse.z = atof(kd.c_str());

				++_next;
			}
		}

		// Ks	Specular Value
		else if (sBuf.find("Ks  ") != string::npos) {
			string value = sBuf;
			BYTE _next = 0;
			value.erase(0, value.find_first_of(" ") + 2);		//Ks Delete

			while (_next < (BYTE)3) {
				string ks;
				ks.append(value, 0, value.find_first_of(" "));
				value.erase(0, value.find_first_of(" ") + 1);

				if (_next == (BYTE)0)
					mMetarial[key]->metarial.Specular.x = atof(ks.c_str());

				else if (_next == (BYTE)1)
					mMetarial[key]->metarial.Specular.y = atof(ks.c_str());

				else if (_next == (BYTE)2)
					mMetarial[key]->metarial.Specular.z = atof(ks.c_str());

				++_next;
			}
		}

		// d	Alpha Value
		else if (sBuf.find("d  ") != string::npos) {
			string value = sBuf;
			value.erase(0, value.find_first_of(" ") + 2);		//Ka Delete
			mMetarial[key]->metarial.Reflect.x = atof(value.c_str());
		}

		// map_Kd	Texture PathName
		else if (sBuf.find("map_Kd ") != string::npos) {
			string fName = sBuf;
			fName.erase(0, fName.find(" ") + 1);
			string _path = path;
			_path.erase(_path.find_last_of("/") + 1, _path.size());
			_path.append(fName);

			wstring wstr;
			wstr.assign(_path.begin(), _path.end());
			LPCWSTR _wPath = wstr.c_str();
			// 문자열 변환
			wstring find_str = L"png";
			wstring replace_str = L"dds";
			wstr.replace(wstr.find(find_str), find_str.length(), replace_str);

			CreateDDSTextureFromFile(g_pDevice, _wPath, nullptr, &(mMetarial[key]->srv));
			assert(mMetarial[key]->srv);
		}
	}

	fclose(mtl);
}

void ObjLoader::Load_OBJ_File(const char* path)
{
	FILE* obj = nullptr;
	fopen_s(&obj, path, "rb");
	assert(obj);
	vector<Vector3> Verticies;
	vector<Vector3> Coord;
	vector<Vector3> Normal;

	//Read File Data
	while (!feof(obj))
	{
		char buffer[1024];
		fgets(buffer, 1024, obj);

		string sBuf = buffer;

		if (sBuf.find("\n") != string::npos)
			sBuf.erase(sBuf.find("\n"), sBuf.size());

		if (sBuf.find("\r") != string::npos)
			sBuf.erase(sBuf.find("\r"), sBuf.size());


		//	#	주석
		if (sBuf.find("#") != string::npos) {}	//None Code

		// mtllib	mtl파일위치 정보
		else if (sBuf.find("mtllib") != string::npos) {

			// Make MTL Path
			string path_mtl = path;
			int delPos = path_mtl.find_last_of("/");
			path_mtl.erase(delPos + 1, path_mtl.size());

			string mtlPath = sBuf;
			mtlPath.erase(0, sBuf.find(" ") + 1);
			mtlPath.erase(0, mtlPath.find("/") + 1);

			path_mtl.append(mtlPath);

			Load_MTL_File(path_mtl.c_str());
		}

		// usemtl	사용할 mtlKey 정보
		else if (sBuf.find("usemtl") != string::npos) {

			string line = sBuf;
			line.erase(0, line.find_first_of(" ") + 1);

			vector<Vertex> _vertex;

			// Surface Maker
			while (true) {

				sBuf.clear();
				fgets(buffer, 1024, obj);
				sBuf = buffer;

				// f		Index of Surface
				if (sBuf.find("f ") != string::npos) {
					string line = sBuf;
					line.erase(0, 2);
					line.erase(line.size() - 1, line.size());

					for (int k = 0; k < 3; ++k)
					{
						_vertex.push_back(Vertex(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector2(0, 0)));

						for (int i = 0; i < 3; ++i)
						{
							string value;

							if (i < 2) 
							{
								value.append(line, 0, line.find_first_of("/"));
								line.erase(0, line.find_first_of("/") + 1);
							}

							else 
							{
								value.append(line, 0, line.find_first_of(" "));
								line.erase(0, line.find_first_of(" ") + 1);
							}

							switch (i)
							{
								case 0:
								{
									int idx = atoi(value.c_str()) - 1;
									assert(idx < Verticies.size());
									_vertex[_vertex.size() - 1].Pos = Verticies[idx];
								}
								break;

								case 1:
								{
									int idx = atoi(value.c_str()) - 1;
									assert(idx < Coord.size());
									_vertex[_vertex.size() - 1].Tex.x = Coord[idx].x;
									_vertex[_vertex.size() - 1].Tex.y = Coord[idx].y;
								}
								break;

								case 2:
								{
									int idx = atoi(value.c_str()) - 1;
									assert(idx < Normal.size());
									_vertex[_vertex.size() - 1].Normal = Normal[idx];
								}
								break;
							}

							value.clear();
						}
					}

				}

				else if (sBuf.find("# ") != string::npos)
					break;
			}

			Object* _new = new Object(g_pDevice, m_pDeviceContext);

			_new->setVertexSize(_vertex.size());
			_new->setVertex(_vertex);

			_new->setMetarial(&(mMetarial.find(line)->second->metarial));

			DirectionalLight dlight;
			dlight.Direction = Vector3(0, -2.0f, 0);
			_new->setDirLight(dlight);

			_new->setTexture(mMetarial.find(line)->second->srv);

			vObject.push_back(_new);
		}

		// v		Vertex Local Position
		else if (sBuf.find("v ") != string::npos) {
			string line = sBuf;
			line.erase(0, 3);

			BYTE _next = 0;
			Vector3 vtx;

			while (_next < (BYTE)3) {
				string value;
				value.append(line, 0, line.find_first_of(" "));
				line.erase(0, line.find_first_of(" ") + 1);

				if (_next == (BYTE)0)
					vtx.x = -atof(value.c_str());

				else if (_next == (BYTE)1)
					vtx.y = atof(value.c_str());

				else if (_next == (BYTE)2)
					vtx.z = atof(value.c_str());

				++_next;
			}

			Verticies.push_back(vtx);
		}

		// vt		Texture Vector Coord
		else if (sBuf.find("vt ") != string::npos) {
			string line = sBuf;
			line.erase(0, 4);

			BYTE _next = 0;
			Vector3 cord;

			while (_next < (BYTE)4) {
				string value;
				value.append(line, 0, line.find_first_of(" "));
				line.erase(0, line.find_first_of(" ") + 1);

				if (_next == (BYTE)0)
					cord.x = atof(value.c_str());

				else if (_next == (BYTE)1)
					cord.y = 1.0f - atof(value.c_str());

				else if (_next == (BYTE)2)
					cord.z = atof(value.c_str());

				++_next;
			}

			Coord.push_back(cord);
		}

		// vn		Normal Vector
		else if (sBuf.find("vn ") != string::npos) {
			string line = sBuf;
			line.erase(0, 4);

			BYTE _next = 0;
			Vector3 norm;

			while (_next < (BYTE)4) {
				string value;
				value.append(line, 0, line.find_first_of(" "));
				line.erase(0, line.find_first_of(" ") + 1);

				if (_next == (BYTE)0)
					norm.x = atof(value.c_str());

				else if (_next == (BYTE)1)
					norm.z = atof(value.c_str());

				else if (_next == (BYTE)2)
					norm.y = atof(value.c_str());

				++_next;
			}

			Normal.push_back(norm);
		}

	}
	fclose(obj);
}

void ObjLoader::RenderParcingObj()
{
	for (auto obj : vObject)
	{
		obj->render();
	}
}
