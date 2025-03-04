#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//=====================================
//             PGM CLASS
//=====================================
class PGMImage {
private:
	int width, height, maxVal;
	string magicNumber;
	unsigned char* grayData; // 1D array for grayscale values

public:
	// Default Constructor
	PGMImage() : width(0), height(0), maxVal(255), grayData(nullptr) {}

	// Parameterized Constructor
	PGMImage(int width, int height, int maxVal, string magicNumber) {
		this->width = width;
		this->height = height;
		this->maxVal = maxVal;
		this->magicNumber = magicNumber;
		grayData = new unsigned char[width * height];
	}
	// Destructor to free memory
	~PGMImage() {
		if (grayData) {
			delete[] grayData;
		}
		grayData = nullptr;
	}


	// Setters
	void setPixel(int index, unsigned char value) {
		if (index >= 0 && index < width * height)
			grayData[index] = value;
	}


	// Function to write a binary PGM (P5) image
	bool writePGM(const string& filename) {
		ofstream file(filename, ios::binary);
		if (!file) {
			cerr << "Error: Unable to create file " << filename << endl;
			return false;
		}

		// Write header
		file << magicNumber << "\n" << width << " " << height << "\n" << maxVal << "\n";

		// Write pixel data
		file.write(reinterpret_cast<char*>(grayData), width * height);

		file.close();
		return true;
	}
};


//===================================================================
//                    PPM CLASS
//===================================================================
class PPMImage {
private:
	int width, height, maxVal;
	string magicNumber;
	unsigned char* Red, * Green, * Blue; // Separate 1D arrays for RGB channels

public:
	// Constructor
	PPMImage() : width(0), height(0), maxVal(255), Red(nullptr), Green(nullptr), Blue(nullptr) {}
	PPMImage(int width, int height, int maxVal, string magicNumber) {

		this->width = width;
		this->height = height;
		this->maxVal = maxVal;
		this->magicNumber = magicNumber;

		int totalPixel = height * width;

		this->Red = new unsigned char[totalPixel];
		this->Green = new unsigned char[totalPixel];
		this->Blue = new unsigned char[totalPixel];
	}

	PPMImage(const string& name) {
		this->readPPM(name);
	}


	int getHeight()const {
		return this->height;
	}
	int getWidth()const {
		return this->width;
	}
	string getMagicNumber()const {
		return this->magicNumber;
	}
	int getMaxVal()const {
		return this->maxVal;
	}
	void clear() {
		if (Red) {
			delete[] Red;
		}
		if (Green) {
			delete[] Green;
		}
		if (Blue) {
			delete[] Blue;
		}
		width = 0;
		height = 0;
		maxVal = 0;
		magicNumber = "";
	}
	// Destructor 
	~PPMImage() {
		if (Red) {
			delete[] Red;
		}
		if (Green) {
			delete[] Green;
		}
		if (Blue) {
			delete[] Blue;
		}
	}

	// Function to read a binary PPM (P6) image
	bool readPPM(const string& filename) {
		ifstream file(filename, ios::binary);
		if (!file) {
			cerr << "Error: Unable to open file " << filename << endl;
			return false;
		}

		string token;
		bool foundMagic = false;
		width = height = maxVal = -1;

		// Read scattered header
		while (file >> token) { // Read word by word
			if (token == "P6") {
				foundMagic = true;
				magicNumber = "P6";
			}
			else if (isdigit(token[0])) { // If it's a number
				if (width == -1) width = stoi(token);
				else if (height == -1) height = stoi(token);
				else if (maxVal == -1) {
					maxVal = stoi(token);
					break; // Stop reading header once we have all parts
				}
			}
		}

		// Validate header
		if (!foundMagic || width <= 0 || height <= 0 || maxVal <= 0) {
			cerr << "Error: Incomplete or invalid PPM header!" << endl;
			return false;
		}

		// Ensure maxVal is within 8-bit range
		if (maxVal > 255) {
			cerr << "Error: Only 8-bit PPM (P6) images are supported." << endl;
			return false;
		}

		file.get(); // Consume one whitespace after the header

		int totalPixels = width * height;

		// Allocate memory for color channels
		Red = new unsigned char[totalPixels];
		Green = new unsigned char[totalPixels];
		Blue = new unsigned char[totalPixels];

		// Read pixel data into separate 1D arrays
		for (int i = 0; i < totalPixels; i++) {
			file.read(reinterpret_cast<char*>(&Red[i]), 1);
			file.read(reinterpret_cast<char*>(&Green[i]), 1);
			file.read(reinterpret_cast<char*>(&Blue[i]), 1);
		}

		file.close();
		return true;
	}

	// Function to write a binary PPM (P6) image
	bool writePPM(string& filename) {
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

	// Function to invert colors
	void invertColors(string output) {
		int totalPixels = width * height;
		for (int i = 0; i < totalPixels; i++) {
			Red[i] = 255 - Red[i];
			Green[i] = 255 - Green[i];
			Blue[i] = 255 - Blue[i];
		}
		output = output + ".ppm";
		this->writePPM(output);
	}

	void minus(const PPMImage& temp, string output) {

		if (this->magicNumber != temp.magicNumber || this->height != temp.height || this->width != temp.width || this->maxVal != temp.maxVal) {
			cerr << "Image format is not compatible! \n";
			return;
		}

		PPMImage product(temp.getWidth(), temp.getHeight(), temp.getMaxVal(), temp.getMagicNumber());

		for (int i = 0; i < product.height * product.width; i++) {
			product.Red[i] = abs(this->Red[i] - temp.Red[i]);
			product.Green[i] = abs(this->Green[i] - temp.Green[i]);
			product.Blue[i] = abs(this->Blue[i] - temp.Blue[i]);
		}
		output = output + ".ppm";

		product.writePPM(output);
	}


	//function to add two images
	void plus(const PPMImage& temp, string output) {
		if (this->magicNumber != temp.magicNumber || this->height != temp.height || this->width != temp.width || this->maxVal != temp.maxVal) {
			cerr << "Image format is not compatible! \n";
			return;
		}

		PPMImage product(temp.getWidth(), temp.getHeight(), temp.getMaxVal(), temp.getMagicNumber());
		for (int i = 0; i < product.height * product.width; i++) {
			product.Red[i] = min((this->Red[i] + temp.Red[i]), product.maxVal);
			product.Green[i] = min((this->Green[i] + temp.Green[i]), product.maxVal);
			product.Blue[i] = min((this->Blue[i] + temp.Blue[i]), product.maxVal);
		}
		output = output + ".ppm";
		product.writePPM(output);
	}

	void makeRed(string output) {
		int len = this->height * this->width;

		for (int i = 0; i < len; i++) {
			this->Green[i] = this->Blue[i] = 0;
		}
		output = output + ".ppm";

		this->writePPM(output);

	}
	void makeGreen(string output) {
		int len = this->height * this->width;

		for (int i = 0; i < len; i++) {
			this->Red[i] = this->Blue[i] = 0;
		}
		output = output + ".ppm";

		this->writePPM(output);

	}
	void makeBlue(string output) {
		int len = this->height * this->width;

		for (int i = 0; i < len; i++) {
			this->Green[i] = this->Red[i] = 0;
		}
		output = output + ".ppm";

		this->writePPM(output);

	}

	void makeYellow(string output) {
		int len = this->height * this->width;
		for (int i = 0; i < len; i++) {
			this->Blue[i] = 0;
			this->Red[i] = this->Green[i] = max(this->Green[i], this->Red[i]);
		}
		output = output + ".ppm";

		this->writePPM(output);
	}

	void makeCyan(string output) {
		int len = this->height * this->width;
		for (int i = 0; i < len; i++) {
			this->Red[i] = 0;
			this->Blue[i] = this->Green[i] = max(this->Blue[i], this->Green[i]);
		}
		output = output + ".ppm";

		this->writePPM(output);
	}

	void makePurple(string output) {
		int len = this->height * this->width;
		for (int i = 0; i < len; i++) {
			this->Green[i] = 0;
			this->Blue[i] = this->Red[i] = max(this->Blue[i], this->Red[i]);
		}
		output = output + ".ppm";

		this->writePPM(output);
	}


	void makeGrey(string output)const {
		PGMImage grey(width, height, maxVal, "P5");

		int len = this->width * this->height;

		for (int i = 0; i < len; i++) {
			grey.setPixel(i, (this->Red[i] + this->Green[i] + this->Blue[i]) / 3);
		}
		output = output + ".pgm";

		grey.writePGM(output);

	}

	void transitions(const PPMImage& temp) {
		if (this->magicNumber != temp.magicNumber || this->height != temp.height || this->width != temp.width || this->maxVal != temp.maxVal) {
			cerr << "Image format is not compatible! \n";
			return;
		}

		PPMImage product(temp.getWidth(), temp.getHeight(), temp.getMaxVal(), temp.getMagicNumber());

		int len = width * height;

		float w = 0.0;
		for (int pass = 0; pass < 10; pass++) {
			for (int i = 0; i < len; i++) {
				product.Red[i] = w * this->Red[i] + (1 - w) * temp.Red[i];
				product.Blue[i] = w * this->Blue[i] + (1 - w) * temp.Blue[i];
				product.Green[i] = w * this->Green[i] + (1 - w) * temp.Green[i];
			}
			string output = "t" + to_string(pass) + ".ppm";
			product.writePPM(output);
			w = w + 0.1;
		}


	}
};



//==========================================================
//                   Menu Functions
//==========================================================
void showMenu() {
	cout << "\n===== Image Processing Menu =====\n";
	cout << "1. Single Image Manipulation\n";
	cout << "2. Two Image Manipulation\n";
	cout << "3. Quit\n";
	cout << "Enter your choice: ";
}

void showSingleImageMenu() {
	cout << "\n--- Single Image Manipulation ---\n";
	cout << "1. Make Red\n";
	cout << "2. Make Green\n";
	cout << "3. Make Blue\n";
	cout << "4. Make Yellow\n";
	cout << "5. Make Cyan\n";
	cout << "6. Make Purple\n";
	cout << "7. Make negative\n";
	cout << "8. Convert to Grayscale\n";
	cout << "9. Back to Main Menu\n";
	cout << "Enter your choice: ";
}

void showTwoImageMenu() {
	cout << "\n--- Two Image Manipulation ---\n";
	cout << "1. Add Images\n";
	cout << "2. Subtract Images\n";
	cout << "3. Make transitions\n";
	cout << "4. Back to Main Menu\n";
	cout << "Enter your choice: ";
}


bool correct(const string& s) {
	return !s.empty() && s.find_first_of("\\/:*?\"<>|") == string::npos && s.find_first_not_of(' ') != string::npos;
}

//========================================================================
//                           MAIN FUNCTION
//========================================================================

int main() {

	int choice;
	while (true) {
		showMenu();
		cin >> choice;

		if (choice == 1) { // Single Image Manipulation
			string filename;
			string output;
			PPMImage image;
			do {
				cout << "Enter image filename (PPM format): ";
				cin >> filename;
			} while (!image.readPPM(filename));

			do {
				cout << "\nEnter Output filename (without extension): ";
				cin >> output;
			} while (!correct(output));

			int singleChoice;
			while (true) {
				showSingleImageMenu();
				cin >> singleChoice;

				if (singleChoice == 1) {
					image.makeRed(output);
				}
				else if (singleChoice == 2) {
					image.makeGreen(output);
				}
				else if (singleChoice == 3) {
					image.makeBlue(output);
				}
				else if (singleChoice == 4) {
					image.makeYellow(output);
				}
				else if (singleChoice == 5) {
					image.makeCyan(output);
				}
				else if (singleChoice == 6) {
					image.makePurple(output);
				}

				else if (singleChoice == 7) {
					image.invertColors(output);
				}
				else if (singleChoice == 8) {
					image.makeGrey(output);
				}
				else if (singleChoice == 9) {
					break; // Go back to main menu
				}
				else {
					cout << "Invalid choice. Try again.\n";
				}
				break;
			}
		}

		else if (choice == 2) { // Two Image Manipulation
			int twoChoice;

			string file1, file2, output;
			PPMImage img1, img2;
			do {
				cout << "Enter first image filename: ";
				cin >> file1;
			} while (!img1.readPPM(file1));
			do {
				cout << "Enter second image filename: ";
				cin >> file2;
			} while (!img2.readPPM(file2));

			while (true) {
				showTwoImageMenu();
				cin >> twoChoice;

				if (twoChoice == 1) { // Add images
					do {
						cout << "\nEnter correct output filename (without extension): ";
						cin >> output;
					} while (!correct(output));

					img1.plus(img2, output);
				}
				else if (twoChoice == 2) { // Subtract images

					do {
						cout << "\nEnter correct output filename (without extension): ";
						cin >> output;
					} while (!correct(output));

					img1.minus(img2, output);
				}
				else if (twoChoice == 3) {

					img1.transitions(img2);
					cout << "Sequence of images saved!\n";
				}
				else if (twoChoice == 4) {
					break; // Go back to main menu
				}
				else {
					cout << "Invalid choice. Try again.\n";
				}
				break;
			}
		}

		else if (choice == 3) {
			cout << "Exiting program...\n";
			break;
		}

		else {
			cout << "Invalid choice. Try again.\n";
		}
	}
}


