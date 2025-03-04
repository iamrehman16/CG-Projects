# Computer Graphics Projects

## 📌 Table of Contents
- [PPM Image Processor](#ppm-image-processor)

---

# PPM Image Processor

## 📌 Overview
This project is a **PPM Reader & Writer** implemented in C++, designed to manipulate binary **PPM (Portable PixMap) images**. It provides functionalities to:
- Apply filters to PPM images
- Add and subtract two images
- Generate an intermediate transition (morphing) between two images

This project was created as part of **Assignment 1** in my Computer Graphics course.

🔗 **Project Folder:** [PPM Image Processor](https://github.com/iamrehman16/CG-Projects/tree/main/Assignment1/CODE/PPM_MAKER)

---

## 🚀 Features
- **PPM Image Reading & Writing**: Load and save binary PPM images efficiently.
- **Image Filters**: Apply basic transformations to enhance or modify images.
- **Image Addition & Subtraction**: Perform pixel-wise arithmetic operations.
- **Image Morphing**: Generate smooth transitions between two PPM images.

---

## 🛠️ Installation & Compilation

### **Prerequisites**
- A C++ compiler (GCC, Clang, MSVC, etc.)
- Make sure you have Git installed if you want to clone the repository.

### **Clone the Repository**
```sh
git clone https://github.com/iamrehman16/CG-Projects.git
cd CG-Projects/Assignment1/CODE/PPM_MAKER
```

### **Compile the Code**
Use any standard C++ compiler to compile the project:
```sh
g++ -o ppm_processor PPM_MAKER.cpp
```

### **Run the Program**
```sh
./ppm_processor input.ppm output.ppm
```
> Replace `input.ppm` and `output.ppm` with actual file names.

---

## 📝 Notes
- The program **only supports binary PPM format (P6)**.
- Ensure that input images have the **same dimensions** when performing addition, subtraction, or morphing.
- Output images are saved in **PPM format**.

---

## 📜 License
This project is for educational purposes and does not have a specific license. Feel free to modify and experiment with it.

---

## 🙌 Contributions & Feedback
Contributions and feedback are welcome! Feel free to open an issue or submit a pull request.

🔗 **GitHub Repository:** [CG-Projects](https://github.com/iamrehman16/CG-Projects)

