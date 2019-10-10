#include "data.h"
#include "vec3.h"
#include "random.h"
using namespace std;

vector<vec3> shape_mu(N_VERTICE);
vector<double> shape_ev(N_PC);
vector<vector<vec3>> shape_pc(N_PC);
vector<vec3> tex_mu(N_VERTICE);
vector<double> tex_ev(N_PC);
vector<vector<vec3>> tex_pc(N_PC);
vector<vec3> tl(N_FACE);

void init();
vector<vec3> coef2object(vector<double> &coef, vector<vec3> &shape_mu, vector<vector<vec3>> &shape_pc, vector<double> &shape_ev);
void plt_write(string fn, vector<vec3> shape, vector<vec3> tex, vector<vec3> tl);

int main()
{
	init();
	if (load(shape_mu, shape_ev, shape_pc,
			 tex_mu, tex_ev, tex_pc, tl) == FAIL) { 
		cout << "failed to load all data, and errors are listed above." << endl;
		cout << "continue or not? [Y/n] ";
		char option;
		while (true) {
			cin >> option;
			if (option == 'Y' || option == 'y')
				break;
			else if (option == 'N' || option == 'n') {
				system("pause");
				return -1;
			}
			else
				cout << "please input 'Y'/'y' or 'N'/'n'." << endl;
		}
	}
	else {
		cout << "load data - success" << endl;
	}

	cout << "generate random sequence - ";
	vector<double> alpha = randn(N_PC);
	vector<double> beta = randn(N_PC);
	cout << "success" << endl;

	cout << "pca - ";
	vector<vec3> shape = coef2object(alpha, shape_mu, shape_pc, shape_ev);
	vector<vec3> tex = coef2object(beta, tex_mu, tex_pc, tex_ev);
	cout << "success" << endl;

	plt_write("random_face.ply", shape, tex, tl);
	system("pause");
    return 0;
}

void init() {
	for (auto it = shape_pc.begin(); it != shape_pc.end(); it++) {
		it->resize(N_VERTICE);
	}
}

vector<vec3> coef2object(vector<double> &coef, vector<vec3> &shape_mu, vector<vector<vec3>> &shape_pc, vector<double> &shape_ev) {
	vector<double> temp = dot(coef, shape_ev);
	vector<vec3> tmp = shape_pc * temp;
	return shape_mu + shape_pc * temp;
}

void plt_write(string fn, vector<vec3> shape, vector<vec3> tex, vector<vec3> tl) {
	ofstream out;
	out.open(fn, std::ios::binary);
	if (!out) {
		cout << "Creation of " << fn << " failed." << endl;
		return;
	}
	out << "ply\n";
	out << "format binary_little_endian 1.0\n";
	out << "comment Made from the 3D Morphable Face Model of the Univeristy of Basel, Switzerland.\n";
	out << "element vertex " << N_VERTICE << "\n";
	out << "property float x\n";
	out << "property float y\n";
	out << "property float z\n";
	out << "property uchar red\n";
	out << "property uchar green\n";
	out << "property uchar blue\n";
	out << "element face " << N_FACE << "\n";
	out << "property list uchar int vertex_indices\n";
	out << "end_header\n";

	for (int i = 0; i < N_VERTICE; i++) {
		float x = float(shape[i].x);
		float y = float(shape[i].y);
		float z = float(shape[i].z);
		unsigned char r = tex[i].x;
		unsigned char g = tex[i].y;
		unsigned char b = tex[i].z;
		out.write((char *)&x, sizeof(x));
		out.write((char *)&y, sizeof(y));
		out.write((char *)&z, sizeof(z));
		out.write((char *)&r, sizeof(r));
		out.write((char *)&g, sizeof(g));
		out.write((char *)&b, sizeof(b));
	}

	unsigned char N_VER_PER_FACE = 3;
	for (int i = 0; i < N_FACE; i++) {
		out.write((char *)&N_VER_PER_FACE, sizeof(N_VER_PER_FACE));
		tl[i] = tl[i] - 1;
		int x = tl[i].x;
		int y = tl[i].y;
		int z = tl[i].z;
		out.write((char *)&y, sizeof(y));
		out.write((char *)&x, sizeof(x));
		out.write((char *)&z, sizeof(z));
	}


	out.close();
}