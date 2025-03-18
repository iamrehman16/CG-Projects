#include <iostream>
#include<fstream>
#include <string>
using namespace std;

struct Point {
	int x, y;
	Point(int a = 0, int b = 0) : x(a), y(b) {}
	/*Point(const Point& p) {
		this->x = p.x;
		this->y = p.y;
	}*/
};

struct Color {
	unsigned char r, g, b;
	Color(unsigned char a = 0, unsigned char b = 0, unsigned char c = 0) : r(a), g(b), b(c) {}
};

struct Face {
	int v1, v2, v3;
	Color c1, c2, c3;

	Face() {
		v1 = v2 = v3 = -1;
	}
};


class Triangle {
private:
	int width, height, maxVal;
	string magicNumber;

	Point* vertices;
	Face* faces;

	int vertexCount, faceCount;
	unsigned char* Red;
	unsigned char* Green;
	unsigned char* Blue;

public:

	Triangle() {
		Red = nullptr;
		Green = nullptr;
		Blue = nullptr;
		magicNumber = "P6";
		maxVal = 255;
		vertices = nullptr;
		faces = nullptr;
	}

	// Destructor
	~Triangle() {
		delete[] vertices;
		delete[] faces;
		delete[] Red;
		delete[] Green;
		delete[] Blue;
	}


	//Reading from a text file
	bool readFromFile(string filename) {
		ifstream file(filename);
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << filename << endl;
			return false;
		}

		string line;

		while (getline(file, line)) {
			if (line == "# image size") {
				file >> width >> height;
			}
			else if (line == "# vertex list") {
				file >> vertexCount;

				// Allocate memory for vertices
				vertices = new Point[vertexCount];

				// Read vertex coordinates
				for (int i = 0; i < vertexCount; i++) {
					file >> vertices[i].x >> vertices[i].y;
				}
			}
			else if (line == "#face list") {
				file >> faceCount;

				// Allocate memory for faces
				faces = new Face[faceCount];

				// Read face indices and colors
				for (int i = 0; i < faceCount; i++) {
					file >> faces[i].v1 >> faces[i].v2 >> faces[i].v3; // Read vertex indices

					int r, g, b;
					file >> r >> g >> b; // Read RGB color
					faces[i].c1.r = static_cast<unsigned char>(r);
					faces[i].c1.g = static_cast<unsigned char>(g);
					faces[i].c1.b = static_cast<unsigned char>(b);

					file >> r >> g >> b; // Read RGB color
					faces[i].c2.r = static_cast<unsigned char>(r);
					faces[i].c2.g = static_cast<unsigned char>(g);
					faces[i].c2.b = static_cast<unsigned char>(b);

					file >> r >> g >> b; // Read RGB color
					faces[i].c3.r = static_cast<unsigned char>(r);
					faces[i].c3.g = static_cast<unsigned char>(g);
					faces[i].c3.b = static_cast<unsigned char>(b);

				}
			}
		}

		file.close();

		// Allocate memory for image arrays
		int len = width * height;
		Red = new unsigned char[len];
		Green = new unsigned char[len];
		Blue = new unsigned char[len];

		// Initialize image with black background
		for (int i = 0; i < len; i++) {
			Red[i] = 0;
			Green[i] = 0;
			Blue[i] = 0;
		}

		return true;
	}




	void colorTriangle() {
		int count = 0;
		for (int f = 0; f < faceCount; f++) {

			//get the current face
			Face face = faces[f];

			//get the vertices of the current triangle
			Point v1 = vertices[face.v1 - 1];
			Point v2 = vertices[face.v2 - 1];
			Point v3 = vertices[face.v3 - 1];

			//get the color of current face
			Color c1 = face.c1;
			Color c2 = face.c2;
			Color c3 = face.c3;

			//compute normal for whole of the triangle
			double n = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);


			int xStart = max(0, (int)min(v1.x, min(v2.x, v3.x)));
			int xEnd = min(width - 1, (int)max(v1.x, max(v2.x, v3.x)));
			int yStart = max(0, (int)min(v1.y, min(v2.y, v3.y)));
			int yEnd = min(height - 1, (int)max(v1.y, max(v2.y, v3.y)));

			//iterate over each pixel in the image
			for (int y = yStart; y < yEnd; y++) {
				for (int x = xStart; x < xEnd; x++) {
					Point p(x, y);

					//compute normals relative to point p(x,y)
					double na = (v3.x - v2.x) * (p.y - v2.y) - (v3.y - v2.y) * (p.x - v2.x);
					double nb = (v1.x - v3.x) * (p.y - v3.y) - (v1.y - v3.y) * (p.x - v3.x);
					double nc = (v2.x - v1.x) * (p.y - v1.y) - (v2.y - v1.y) * (p.x - v1.x);

					//compute barycentric weights
					double alpha = na / n;
					double beta = nb / n;
					double gamma = nc / n;

					//check if the pixel is inside the triangle
					if (alpha >= 0 && beta >= 0 && gamma >= 0 && alpha <= 1 && beta <= 1 && gamma <= 1) {
						int index = y * width + x;
						Red[index] = c1.r * alpha + c2.r * beta + c3.r * gamma;
						Green[index] = c1.g * alpha + c2.g * beta + c3.g * gamma;
						Blue[index] = c1.b * alpha + c2.b * beta + c3.b * gamma;
					}
				}
			}
		}
	}


	//function to write image as ppm
	bool writePPM(string filename) {
		ofstream file(filename, ios::binary);
		if (!file) {
			cerr << "Error: Unable to create file " << filename << endl;
			return false;
		}

		// Construct header
		file << magicNumber << "\n" << width << " " << height << "\n" << maxVal << "\n";

		int totalPixels = width * height;

		// Write pixel data
		for (int i = 0; i < totalPixels; i++) {
			file.write(reinterpret_cast<char*>(&Red[i]), 1);
			file.write(reinterpret_cast<char*>(&Green[i]), 1);
			file.write(reinterpret_cast<char*>(&Blue[i]), 1);
		}

		file.close();

		cout << "Image saved as " << filename << endl;
		return true;
	}

};


int main() {
	Triangle triangle;
	string inputFile, outputFile;

	cout << "=== Triangle PPM Generator ===\n";

	//get input file
	while (true) {
		cout << "Enter the input text file name: ";
		cin >> inputFile;

		ifstream testFile(inputFile);
		if (!testFile) {
			cerr << "Error: File not found or cannot be opened. Try again.\n";
		}
		else {
			testFile.close();
			break;
		}
	}

	//read from file
	if (!triangle.readFromFile(inputFile)) {
		cerr << "Failed to read file. Exiting...\n";
		return 1;
	}

	//get output file name
	cout << "Enter the output PPM file name (with .ppm extension): ";
	cin >> outputFile;

	//ensure the file has a .ppm extension and if not then add .ppm
	if (outputFile.size() < 4 || outputFile.substr(outputFile.size() - 4) != ".ppm") {
		outputFile += ".ppm";
	}

	//process and generate PPM file
	triangle.colorTriangle();
	if (!triangle.writePPM(outputFile)) {
		cerr << "Failed to write PPM file. Exiting...\n";
		return 1;
	}

	cout << "PPM file successfully generated as " << outputFile << "\n";
}
