#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "helpers.h"
#include "mlp_classifier.h"

using namespace mnist;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " test.csv model_dir\n";
        return 1;
    }
    cout << "Multilayer perceptron." << endl << "Start calculation of the model quality (accuracy)." << endl;

    string test_path = string(argv[1]) + "/test.csv";
    string model_dir = argv[2];
    string w1_path = model_dir + "/w1.txt";
    string w2_path = model_dir + "/w2.txt";

    // 1. Загрузка весов
    Eigen::MatrixXf w1 = read_mat_from_file(784, 128, w1_path);
    Eigen::MatrixXf w2 = read_mat_from_file(128, 10, w2_path);

    // 2. Транспонируем
    w1.transposeInPlace(); // → [128 x 784]
    w2.transposeInPlace(); // → [10 x 128]

    // 3. инициализируем классификатор
    MlpClassifier model(w1, w2);

    ifstream test_file(test_path);
    if (!test_file)
    {
        cerr << "Failed to open " << test_path << endl;
        return 1;
    }

    size_t correct = 0;
    size_t total = 0;

    string line;
    while (getline(test_file, line))
    {
        stringstream ss(line);
        string token;

        // Сначала читаем класс (label)
        getline(ss, token, ',');
        int label = stoi(token);

        // Потом читаем пиксели (признаки)
        mnist::Classifier::features_t features;
        while (getline(ss, token, ','))
        {
            features.push_back(stof(token));
        }

        if (features.size() != 784)
        {
            cerr << "Invalid feature size: " << features.size() << endl;
            continue;
        }

        int prediction = model.predict(features);

        if (prediction == label)
        {
            ++correct;
        }
        ++total;
    }

    // 5. Accuracy
    if (total == 0)
    {
        cerr << "No data found in test file.\n";
        return 1;
    }

    float accuracy = static_cast<float>(correct) / total;
    cout << "Accuracy = " + to_string(accuracy) << endl;

    return 0;
}
