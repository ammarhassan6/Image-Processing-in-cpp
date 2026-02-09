#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdlib>

using namespace std;

struct Image {
    char ImageFileName[100];
    vector<vector<int>> data_for_image;
    int cols, rows, maxGray;
    vector<char> comment;

    bool imageLoaded;
    bool imageModified;


    int imageloader(char ImageName[]) {
        ifstream FCIN(ImageName, ios::in);

        if (!FCIN.is_open())
            return -1;

        char MagicNumber[5];
        char com[100];          //comment

        FCIN.getline(MagicNumber, 4);
        FCIN.getline(com, 100);
        FCIN >> cols >> rows >> maxGray;

        data_for_image.clear();
        data_for_image.resize(rows, vector<int>(cols, 0));

        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++)
                FCIN >> data_for_image[r][c];

        if (FCIN.fail())
            return -2;

        FCIN.close();
        imageLoaded = true;
        imageModified = false;
        strcpy_s(ImageFileName, ImageName);
        return 0;
    }

    int Imagesaver(char ImageName[]) {
        ofstream FCOUT(ImageName, ios::out);
        if (!FCOUT.is_open())
            return -1;

        FCOUT << "P2\n# This is a comment\n"
            << cols << " " << rows << endl
            << maxGray << endl;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++)
                FCOUT << data_for_image[r][c] << " ";
            FCOUT << endl;
        }
        FCOUT.close();
        imageModified = false;
        return 0;
    }
    void showImage() const {
        if (!imageLoaded) {
            cout << "Error: No image loaded." << endl;
            return;
        }

        const char* viewerCommand = "start";


        string command = string(viewerCommand) + " " + ImageFileName;

        int result = system(command.c_str());

        if (result == 0) {
            cout << "Image displayed using default viewer." << endl;
        }
        else {
            cout << "Error: Unable to open image using the default viewer." << endl;
        }
    }

    void horizontalFlip() {
        int temp;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols / 2; j++) {
                temp = data_for_image[i][j];
                data_for_image[i][j] = data_for_image[i][cols - j - 1];
                data_for_image[i][cols - j - 1] = temp;
            }
        }
    }

    void meanFilter() {
        int t_matrix = 3;
        int T_matrixsize = t_matrix * t_matrix;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                int sum = 0;
                for (int i = -t_matrix / 2; i <= t_matrix / 2; i++) {
                    for (int j = -t_matrix / 2; j <= t_matrix / 2; j++) {
                        int height = r + i;
                        int width = c + j;
                        if (height >= 0 && height < rows && width >= 0 && width < cols) {
                            sum += data_for_image[height][width];
                        }
                    }
                }
                data_for_image[r][c] = sum / T_matrixsize;
            }
        }
    }

    void verticalFlipImage() {
        for (int r = 0; r < rows / 2; r++)
            for (int c = 0; c < cols; c++) {
                int T = data_for_image[r][c];
                data_for_image[r][c] = data_for_image[rows - 1 - r][c];
                data_for_image[rows - 1 - r][c] = T;
            }
        return;
    }

    void adjustSharpness(double strength) {
        vector<vector<int>> tempData = data_for_image;

        for (int r = 1; r < rows - 1; ++r) {
            for (int c = 1; c < cols - 1; ++c) {
                int newValue = static_cast<int>(
                    (data_for_image[r][c] + (data_for_image[r][c] - data_for_image[r - 1][c - 1]) * strength)
                    );

                tempData[r][c] = max(0, min(maxGray, newValue));
            }
        }

        data_for_image = tempData;
        imageModified = true;
    }

    void contrastStretching(double factor) {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                data_for_image[r][c] = static_cast<int>((data_for_image[r][c] - maxGray / 2) * factor + maxGray / 2);

                data_for_image[r][c] = max(0, min(maxGray, data_for_image[r][c]));
            }
        }
        imageModified = true;
    }

    void changeBrightness(double factor) {
        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++) {
                data_for_image[r][c] *= factor;
                if (data_for_image[r][c] > maxGray)
                    data_for_image[r][c] = maxGray;
            }
    }

    void TranslateImage() {
        int xShift, yShift;
        cout << "Enter the x translation: ";
        cin >> xShift;
        cout << "Enter the y translation: ";
        cin >> yShift;

        vector<vector<int>> newData(rows, vector<int>(cols, 0));

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int newR = r + yShift;
                int newC = c + xShift;

                if (newR >= 0 && newR < rows && newC >= 0 && newC < cols) {
                    newData[newR][newC] = data_for_image[r][c];
                }
            }
        }

        cout << "Translated Image Dimensions: " << newData.size() << " x " << newData[0].size() << endl;

        data_for_image = newData;

        rows = newData.size();
        cols = newData[0].size();

        cout << "Image translated.\n";
    }

    void RotateImageClockwise() {
        vector<vector<int>> temp(cols, vector<int>(rows, 0));

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                temp[j][rows - i - 1] = data_for_image[i][j];
            }
        }

        data_for_image = temp;
    }

    void rotateImageAntiClockwise() {
        vector<vector<int>> temp(cols, vector<int>(rows, 0));

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                temp[j][i] = data_for_image[i][j];
            }
        }

        data_for_image = temp;

        int swap = rows;
        rows = cols;
        cols = swap;
    }

    void rotateImageByAngle(double theta) {
        const double PI = 3.141592;
        double angleInRadians = theta * PI / 180.0;

        double cosTheta = cos(angleInRadians);
        double sinTheta = sin(angleInRadians);

        double centerX = static_cast<double>(cols) / 2.0;
        double centerY = static_cast<double>(rows) / 2.0;

        int rotatedCols = abs(cosTheta) * cols + abs(sinTheta) * rows;
        int rotatedRows = abs(sinTheta) * cols + abs(cosTheta) * rows;

        vector<vector<int>> rotatedData(rotatedRows, vector<int>(rotatedCols, 0));

        for (int r = 0; r < rotatedRows; ++r) {
            for (int c = 0; c < rotatedCols; ++c) {
                double x = c - centerX - (rotatedCols - cols) / 2.0;
                double y = r - centerY - (rotatedRows - rows) / 2.0;

                double originalX = x * cosTheta - y * sinTheta + centerX;
                double originalY = x * sinTheta + y * cosTheta + centerY;

                int x0 = originalX;
                int y0 = originalY;
                int x1 = x0 + 1;
                int y1 = y0 + 1;

                if (x0 >= 0 && x1 < cols && y0 >= 0 && y1 < rows) {
                    double weightX1 = originalX - x0;
                    double weightX0 = 1.0 - weightX1;
                    double weightY1 = originalY - y0;
                    double weightY0 = 1.0 - weightY1;

                    rotatedData[r][c] = weightY0 * (weightX0 * data_for_image[y0][x0] + weightX1 * data_for_image[y0][x1])
                        + weightY1 * (weightX0 * data_for_image[y1][x0] + weightX1 * data_for_image[y1][x1]);
                }
            }
        }

        data_for_image = rotatedData;
        cols = rotatedCols;
        rows = rotatedRows;
        imageModified = true;
    }

    void applyMedianFilter() {
        vector<vector<int>> tempData = data_for_image;

        for (int r = 1; r < rows - 1; ++r) {
            for (int c = 1; c < cols - 1; ++c) {

                int neighborhood[9] = {
                    data_for_image[r - 1][c - 1], data_for_image[r - 1][c], data_for_image[r - 1][c + 1],
                    data_for_image[r][c - 1], data_for_image[r][c], data_for_image[r][c + 1],
                    data_for_image[r + 1][c - 1], data_for_image[r + 1][c], data_for_image[r + 1][c + 1]
                };
                for (int i = 0; i < 5; ++i) {
                    int minIndex = i;
                    for (int j = i + 1; j < 9; ++j) {
                        if (neighborhood[j] < neighborhood[minIndex]) {
                            minIndex = j;
                        }
                    }

                    swap(neighborhood[i], neighborhood[minIndex]);
                }
                int medianValue = neighborhood[4];
                tempData[r][c] = max(0, min(maxGray, medianValue));
            }
        }

        data_for_image = tempData;
        imageModified = true;
    }

    void  convertToBinary() {
        int threshold = 128;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (data_for_image[r][c] > threshold) {
                    data_for_image[r][c] = 1;
                }
                else {
                    data_for_image[r][c] = 0;
                }
            }
        }
        imageModified = true;
    }

    void scalingImage(double Factor) {
        int newCols = static_cast<int>(cols * Factor);
        int newRows = static_cast<int>(rows * Factor);
        vector<vector<int>> newData(newRows, vector<int>(newCols, 0));

        for (int r = 0; r < newRows; ++r)
        {
            for (int c = 0; c < newCols; ++c)
            {

                int origR = static_cast<int>(r / Factor);
                int origC = static_cast<int>(c / Factor);


                newData[r][c] = data_for_image[origR][origC];
            }
        }

        cols = newCols;
        rows = newRows;
        data_for_image = newData;
    }

    void cropImage(int cropWidth, int cropHeight) {

        if (cropWidth <= 0 || cropHeight <= 0 || cropWidth > cols || cropHeight > rows) {
            cout << "Invalid crop dimensions. Please make sure the dimensions are within the image bounds." << endl;
            return;
        }

        vector<vector<int>> croppedData(cropHeight, vector<int>(cropWidth, 0));

        for (int r = 0; r < cropHeight; ++r) {
            for (int c = 0; c < cropWidth; ++c) {
                croppedData[r][c] = data_for_image[r][c];
            }
        }

        data_for_image = croppedData;
        rows = cropHeight;
        cols = cropWidth;
        imageModified = true;
    }

    void mergeImagesSideBySide(const Image& otherImage) {
        if (!imageLoaded || !otherImage.imageLoaded) {
            cout << "Both images must be loaded to perform this operation." << endl;
            return;
        }
        int newRows = max(rows, otherImage.rows);
        int newCols = cols + otherImage.cols;

        vector<vector<int>> mergedData(newRows, vector<int>(newCols, 0));

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                mergedData[r][c] = data_for_image[r][c];
            }
        }
        for (int r = 0; r < otherImage.rows; ++r) {
            for (int c = 0; c < otherImage.cols; ++c) {
                mergedData[r][cols + c] = otherImage.data_for_image[r][c];
            }
        }

        data_for_image = mergedData;
        rows = newRows;
        cols = newCols;

        imageModified = true;
        cout << "Images merged side by side. You need to save the changes." << endl;
    }

    void mergeImagesTopToBottom(const Image& otherImage) {
        if (!imageLoaded || !otherImage.imageLoaded) {
            cout << "Both images must be loaded to perform this operation." << endl;
            return;
        }
        int newRows = rows + otherImage.rows;
        int newCols = max(cols, otherImage.cols);

        vector<vector<int>> mergedData(newRows, vector<int>(newCols, 0));

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                mergedData[r][c] = data_for_image[r][c];
            }
        }

        for (int r = 0; r < otherImage.rows; ++r) {
            for (int c = 0; c < otherImage.cols; ++c) {
                mergedData[rows + r][c] = otherImage.data_for_image[r][c];
            }
        }

        data_for_image = mergedData;
        rows = newRows;
        cols = newCols;

        imageModified = true;
        cout << "Images merged top to bottom. You need to save the changes." << endl;
    }

    void resizeImage(int newWidth, int newHeight) {

        vector<vector<int>> resizedData(newHeight, vector<int>(newWidth, 0));

        double widthScale = static_cast<double>(cols) / newWidth;
        double heightScale = static_cast<double>(rows) / newHeight;

        for (int r = 0; r < newHeight; ++r) {
            for (int c = 0; c < newWidth; ++c) {
                double originalX = c * widthScale;
                double originalY = r * heightScale;

                int x0 = static_cast<int>(floor(originalX));
                int y0 = static_cast<int>(floor(originalY));
                int x1 = x0 + 1;
                int y1 = y0 + 1;

                if (x0 >= 0 && x1 < cols && y0 >= 0 && y1 < rows) {
                    double weightX1 = originalX - x0;
                    double weightX0 = 1.0 - weightX1;
                    double weightY1 = originalY - y0;
                    double weightY0 = 1.0 - weightY1;

                    resizedData[r][c] = static_cast<int>(
                        weightY0 * (weightX0 * data_for_image[y0][x0] + weightX1 * data_for_image[y0][x1]) +
                        weightY1 * (weightX0 * data_for_image[y1][x0] + weightX1 * data_for_image[y1][x1])
                        );
                }
            }
        }

        data_for_image = resizedData;
        cols = newWidth;
        rows = newHeight;
        imageModified = true;
    }

    int linearFilter(char ImageName[]) {
        ifstream file(ImageName);
        if (!file.is_open()) {
            cout << "Error: could not open filter file" << endl;
            return 0;
        }
        int Size;
        file >> Size;
        vector<vector<double>> filter(Size, vector<double>(Size));
        for (int i = 0; i < Size; i++) {
            for (int j = 0; j < Size; j++) {
                file >> filter[i][j];
            }
        }
        file.close();
        vector<vector<int>> filteredImage(rows, vector<int>(cols));
        int Center = Size / 2;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                double sum = 0.0;
                for (int i = 0; i < Size; i++) {
                    for (int j = 0; j < Size; j++) {
                        int f = r - Center + i;
                        int k = c - Center + j;
                        if (f >= 0 && f < rows && k >= 0 && k < cols) {
                            sum += data_for_image[f][k] * filter[i][j];
                        }
                    }
                }
                filteredImage[r][c] = static_cast<int>(sum);
            }
        }
        data_for_image = filteredImage;

        return 1;

    }

    void enhanceImage() {
        vector<vector<int>> enhancedData(rows, vector<int>(cols, 0));
        vector<vector<int>> laplacianFilter = { {0, -1, 0}, {-1, 4, -1}, {0, -1, 0} };

        for (int r = 1; r < rows - 1; ++r) {
            for (int c = 1; c < cols - 1; ++c) {
                int sum = 0;

                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        sum += laplacianFilter[i + 1][j + 1] * data_for_image[r + i][c + j];
                    }
                }

                enhancedData[r][c] = max(0, min(maxGray, data_for_image[r][c] + sum));
            }
        }

        data_for_image = enhancedData;
        imageModified = true;
    }

    void edgeDetection() {
        vector<vector<int>> sobel_x = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };

        vector<vector<int>> sobel_y = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

        vector<vector<int>> derivative_x = convolution(sobel_x);
        vector<vector<int>> derivative_y = convolution(sobel_y);

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int magnitude = sqrt(derivative_x[r][c] * derivative_x[r][c] + derivative_y[r][c] * derivative_y[r][c]);
                data_for_image[r][c] = min(maxGray, magnitude);
            }
        }

        imageModified = true;
    }

    vector<vector<int>> convolution(const vector<vector<int>>& filter) {
        int filterSize = filter.size();
        vector<vector<int>> result(rows, vector<int>(cols, 0));

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int sum = 0;

                for (int i = -filterSize / 2; i <= filterSize / 2; ++i) {
                    for (int j = -filterSize / 2; j <= filterSize / 2; ++j) {
                        int height = r + i;
                        int width = c + j;

                        if (height >= 0 && height < rows && width >= 0 && width < cols) {
                            sum += data_for_image[height][width] * filter[i + filterSize / 2][j + filterSize / 2];
                        }
                    }
                }

                result[r][c] = sum;
            }
        }

        return result;
    }

    void computeDerivative() {
        if (!imageLoaded) {
            cout << "Error: Image not loaded." << endl;
            return;
        }


        int mask[3][3] = { {-1, -1, -1},
                              {0, 0, 0},
                              {1, 0, 1}
        };

        vector<vector<int>> derivativeImageData(rows, vector<int>(cols, 0));

        for (int i = 1; i < rows - 1; ++i) {
            for (int j = 1; j < cols - 1; ++j) {
                int sum = 0;

                for (int ni = -1; ni <= 1; ++ni) {
                    for (int nj = -1; nj <= 1; ++nj) {
                        sum += data_for_image[i + ni][j + nj] * mask[ni + 1][nj + 1];
                    }
                }

                derivativeImageData[i][j] = sum;
            }
        }

        data_for_image = derivativeImageData;

        cout << " Derivative Computed ." << endl;
    }
};

struct Menu {
    vector<string> menuItems;

    Menu(char menuFile[]) {
        loadMenu(menuFile);
    }

    int loadMenu(char menuFile[]) {
        ifstream IN;
        IN.open(menuFile, ios::in);
        if (!IN.is_open())
            return -1;
        char menuItem[100], TotalItems[10];

        int Choices;

        IN.getline(TotalItems, 10);
        Choices = atoi(TotalItems);
        for (int i = 1; i <= Choices; i++) {
            IN.getline(menuItem, 99);
            menuItems.push_back(menuItem);
        }
        IN.close();
        return Choices;
    }

    int presentMenu() {
        int userChoice;
        int totalChoices = menuItems.size();

        do {
            int k = 1;
            for (int i = 0; i < totalChoices; i++) {
                if (menuItems[i][0] != '*') {
                    cout << k << "\t" << menuItems[i] << endl;
                    k++;
                }
            }
            cout << " Enter Your Choice (1 - " << k - 1 << " ) ";
            cin >> userChoice;
        } while (userChoice < 1 || userChoice > totalChoices);
        return userChoice;
    }
};

int main() {
    char MenuFile[] = "MainMenu.txt";
    Image images[2];
    int activeImage = 0;
    int errorCode = 0;
    int userChoice;
    int totalChoices;

    Menu menu(MenuFile);
    totalChoices = menu.menuItems.size();
    do {
        userChoice = menu.presentMenu();
        if (1 == userChoice) {
            char ImageFileName[100];
            cout << "Specify File Name ";
            cin >> ImageFileName;
            errorCode = images[activeImage].imageloader(ImageFileName);
            if (errorCode == 0) {
                cout << "\nFile Loaded Successfully.\n" << endl;
            }
            else {
                cout << "\nLoad Error: Code.\n" << errorCode << endl;
            }
        }
        else if (2 == userChoice) {
            char ImageFileName[100];
            cout << "\nSpecify File Name: ";
            cin >> ImageFileName;
            errorCode = images[activeImage].Imagesaver(ImageFileName);
            if (errorCode == 0) {
                cout << "\nFile Saved as " << ImageFileName << endl;
            }
            else {
                cout << "\nSave Error: Code " << errorCode << endl;
            }
        }
        else if (3 == userChoice) {
            double factor = 0;
            cout << "Enter the factor to increase brightness ( 1.0 - 2.0 ) : ";
            cin >> factor;
            images[activeImage].changeBrightness(factor);
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            cout << "Press 2 to save the changes for brightness. " << endl;
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
        }
        else if (4 == userChoice) {
            double factor = 0;
            cout << "Enter the factor ( 1.0 - 2.0 ) : ";
            cin >> factor;
            images[activeImage].contrastStretching(factor);
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            cout << "Press 2 to save the changes. " << endl;
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
        }
        else if (5 == userChoice) {
            double sharpnessFactor;
            cout << "Enter Sharpness Factor: ";
            cin >> sharpnessFactor;
            if (sharpnessFactor < 0.1 || sharpnessFactor > 1.0) {
                cout << "Invalid sharpness factor. Please enter a value between 0.1 and 1.0." << endl;
            }
            else {
                images[activeImage].adjustSharpness(sharpnessFactor);
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
                cout << "Press 2 to save the changes. " << endl;
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            }
        }
        else if (6 == userChoice) {
            images[activeImage].convertToBinary();
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            cout << "Press 2 to save text file of binary " << endl;
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
        }
        else if (7 == userChoice) {
            int newWidth, newHeight;
            cout << "Enter the new width: ";
            cin >> newWidth;
            cout << "Enter the new height: ";
            cin >> newHeight;

            images[activeImage].resizeImage(newWidth, newHeight);
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            cout << "Image resized. Press 2 to save the changes. " << endl;
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
        }
        else if (8 == userChoice) {
            int option = 0;
            cout << "1.Clockwise Rotation \n2.Anticlockwise Rotation \n3.At Specific Angle" << endl;
            cin >> option;
            if (option == 1) {
                images[activeImage].RotateImageClockwise();
                int swap = images[activeImage].rows;
                images[activeImage].rows = images[activeImage].cols;
                images[activeImage].cols = swap;
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
                cout << " Image Rotated. Press 2 to save the changes. " << endl;
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            }
            if (option == 2) {
                images[activeImage].rotateImageAntiClockwise();
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
                cout << " Immage Rotated. You need to save the changes " << endl;
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            }
            if (option == 3) {
                double angle = 0;
                cout << "(Enter +ve angle for forward rotation & -ve angle for reverse rotation): ";
                cin >> angle;
                images[activeImage].rotateImageByAngle(angle);
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
                cout << " Image Rotated. You need to save the changes " << endl;
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            }
        }
        else if (9 == userChoice) {
            int option = 0;
            cout << "1.Horizontal Flip \n2.Vertical Flip " << endl;
            cin >> option;
            if (option == 1) {
                images[activeImage].horizontalFlip();
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
                cout << " Image Flipped. You need to save the changes " << endl;
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            }
            if (option == 2) {
                images[activeImage].verticalFlipImage();
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
                cout << " Image Flipped. Press 2 to save the changes. " << endl;
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            }
        }
        else if (10 == userChoice) {
            int width = 0, height = 0;
            cout << "Enter width to crop: ";
            cin >> width;
            cout << "Enter height to crop: ";
            cin >> height;
            images[activeImage].cropImage(width, height);
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            cout << " Image Cropped. Press 2 to save the changes." << endl;
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
        }
        else if (11 == userChoice) {
            int choice = 0;
            cout << "Note: Before merging, upload first image by upload function and second imagae while running the merge function." << endl;
            cout << "1.Side by Side\n2.Top to Bottom" << endl << "Enter choice: ";
            cin >> choice;
            if (choice == 1) {
                char otherImageFileName[100];
                cout << "Specify the file name of the second image to merge: ";
                cin >> otherImageFileName;

                Image otherImage;
                int errorCode = otherImage.imageloader(otherImageFileName);

                if (errorCode == 0) {
                    images[activeImage].mergeImagesSideBySide(otherImage);
                }
                else {
                    cout << "Load Error for the second image: Code " << errorCode << endl;
                }
            }
            if (choice == 2) {
                char otherImageFileName[100];
                cout << "Specify the file name of the second image to merge: ";
                cin >> otherImageFileName;

                Image otherImage;
                int errorCode = otherImage.imageloader(otherImageFileName);

                if (errorCode == 0) {
                    images[activeImage].mergeImagesTopToBottom(otherImage);
                }
                else {
                    cout << "Load Error for the second image: Code " << errorCode << endl;
                }
            }
        }
        else if (12 == userChoice) {
            int choice = 0;
            cout << "1.Mean Filter \n2.Median Filter" << endl;
            cout << "Enter choice: ";
            cin >> choice;
            if (choice == 1) {
                images[activeImage].meanFilter();
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
                cout << "Press 2 to save the changes. " << endl;
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            }
            if (choice == 2) {
                images[activeImage].applyMedianFilter();
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
                cout << "Press 2 to save the changes." << endl;
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            }
        }
        else if (13 == userChoice) {
            char filter[100];
            cout << "Specify File Name ";
            cin >> filter;
            int errorCode = images[activeImage].linearFilter(filter);
            if (errorCode == 0) {
                cout << "Error: Unable to open filter file." << endl;
            }
            else {
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
                cout << "Filter applied successfully." << endl;
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            }
        }
        else if (14 == userChoice) {
            images[activeImage].enhanceImage();
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            cout << "Press 2 to save the changes. " << endl;
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl;
        }
        else if (15 == userChoice) {
            images[activeImage].computeDerivative();
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            cout << "Press 2 to save the changes." << endl;
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl;
        }
        else if (16 == userChoice) {
            images[activeImage].edgeDetection();
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            cout << "Edge detection applied. Press 2 to save the changes." << endl;
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl;
        }
        else if (17 == userChoice) {
            images[activeImage].TranslateImage();
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            cout << " Image Translated. Press 2 to save the translation changes." << endl << endl;
            cout << " _*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*" << endl;
        }
        else if (18 == userChoice) {
            double factor = 0;
            cout << "Enter Scaling factor: ";
            cin >> factor;
            images[activeImage].scalingImage(factor);
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
            cout << "Press 2 to save the scaling changes." << endl;
            cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl;
        }
        else if (19 == userChoice) {
            if (errorCode == 0) {
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
                cout << "\nFile Loaded Successfully.\n" << endl;
                cout << "_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_* " << endl << endl;
                images[activeImage].showImage();  // Show the loaded image
            }
            else {
                cout << "\nLoad Error: Code.\n" << errorCode << endl;
            }
        }
    } while (userChoice != totalChoices);
    return 0;
}
