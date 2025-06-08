#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <iomanip>
#include <fstream>

// Struktur untuk menyimpan hasil regresi
struct RegressionResult {
    double a0_intercept;
    double a1_slope;
    double r_squared;
    double correlation;
};

// Fungsi untuk melakukan regresi linier
RegressionResult linearRegression(const std::vector<double>& x, 
                                  const std::vector<double>& y) {
    int n = x.size();
    if (n == 0 || n != y.size()) {
        return {0, 0, 0, 0}; // Penanganan kasus error
    }

    double sum_x = 0.0, sum_y = 0.0;
    double sum_xy = 0.0, sum_x2 = 0.0, sum_y2 = 0.0;

    for (int i = 0; i < n; ++i) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x2 += x[i] * x[i];
        sum_y2 += y[i] * y[i];
    }

    double x_mean = sum_x / n;
    double y_mean = sum_y / n;

    // Menghitung a1 (slope) dan a0 (intercept)
    double numerator = n * sum_xy - sum_x * sum_y;
    double denominator = n * sum_x2 - sum_x * sum_x;

    RegressionResult result;
    result.a1_slope = numerator / denominator;
    result.a0_intercept = y_mean - result.a1_slope * x_mean;

    // Menghitung koefisien korelasi dan R²
    double SST = sum_y2 - (sum_y * sum_y / n); // Total sum of squares
    double SSR = 0.0; // Regression sum of squares
    for (int i = 0; i < n; ++i) {
        double y_pred = result.a0_intercept + result.a1_slope * x[i];
        SSR += pow(y_pred - y_mean, 2);
    }
    
    result.r_squared = SSR / SST;
    result.correlation = sqrt(result.r_squared) * ((result.a1_slope >= 0) ? 1 : -1);

    return result;
}

// Fungsi untuk menghitung nilai prediksi berdasarkan model regresi
std::vector<double> calculatePredictions(const std::vector<double>& x, 
                                         double intercept, 
                                         double slope) {
    std::vector<double> predictions;
    for (double x_val : x) {
        predictions.push_back(intercept + slope * x_val);
    }
    return predictions;
}

// Fungsi untuk menyimpan hasil ke file CSV
void saveResultsToCSV(const std::vector<double>& x, 
                      const std::vector<double>& y,
                      const std::vector<double>& y_pred,
                      const std::string& filename) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    
    outfile << "x,y_actual,y_predicted\n";
    for (size_t i = 0; i < x.size(); ++i) {
        outfile << x[i] << "," << y[i] << "," << y_pred[i] << "\n";
    }
    outfile.close();
    std::cout << "Results saved to " << filename << std::endl;
}

int main() {
    std::cout << "===============================================" << std::endl;
    std::cout << "APLIKASI REGRESI LINIER UNTUK PENENTUAN PARAMETER" << std::endl;
    std::cout << "      PADA MODEL KINETIKA PERTUMBUHAN MIKROBA" << std::endl;
    std::cout << "===============================================" << std::endl;

    // Pilihan untuk menggunakan data sampel atau memasukkan data sendiri
    char choice;
    std::cout << "Gunakan data sampel? (y/n): ";
    std::cin >> choice;

    std::vector<double> x_data, y_data;

    if (choice == 'y' || choice == 'Y') {
        // Data sampel yang sudah ditransformasi (1/f vs 1/k)
        x_data = {
            0.14286, 0.11111, 0.06667, 0.04000, 
            0.02500, 0.01333, 0.01000, 0.00667
        };
        y_data = {
            3.448, 2.703, 2.083, 1.538, 1.250, 
            1.031, 1.010, 0.935
        };
        
        std::cout << "\nMenggunakan data sampel dari model Monod:" << std::endl;
        std::cout << "x (1/f)\ty (1/k)" << std::endl;
        std::cout << "----------------" << std::endl;
        for (size_t i = 0; i < x_data.size(); ++i) {
            std::cout << std::fixed << std::setprecision(5);
            std::cout << x_data[i] << "\t" << y_data[i] << std::endl;
        }
    } else {
        int n;
        std::cout << "Masukkan jumlah data: ";
        std::cin >> n;

        std::cout << "Masukkan pasangan nilai x (1/f) dan y (1/k):" << std::endl;
        for (int i = 0; i < n; ++i) {
            double x, y;
            std::cout << "Data #" << (i+1) << " - x: ";
            std::cin >> x;
            std::cout << "Data #" << (i+1) << " - y: ";
            std::cin >> y;
            x_data.push_back(x);
            y_data.push_back(y);
        }
    }

    // Melakukan regresi linier
    RegressionResult res = linearRegression(x_data, y_data);

    // Menghitung parameter kinetika k_max dan K
    double k_max = 1 / res.a0_intercept;
    double K = res.a1_slope * k_max;

    // Menghitung nilai prediksi
    std::vector<double> y_pred = calculatePredictions(x_data, res.a0_intercept, res.a1_slope);

    // Menampilkan hasil dengan format presisi
    std::cout << "\n--- Hasil Regresi Linier ---" << std::endl;
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Persamaan: y = " << res.a0_intercept << " + " << res.a1_slope << "x" << std::endl;
    std::cout << "Koefisien determinasi (R²): " << res.r_squared << std::endl;
    std::cout << "Koefisien korelasi (r): " << res.correlation << std::endl;
    
    std::cout << "\n--- Parameter Kinetika Hasil Perhitungan ---" << std::endl;
    std::cout << "k_max = " << k_max << " day^-1" << std::endl;
    std::cout << "K = " << K << " mg/L" << std::endl;

    std::cout << "\n--- Data dan Hasil Prediksi ---" << std::endl;
    std::cout << "x (1/f)\ty (1/k)\ty_pred\tResidual" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    
    for (size_t i = 0; i < x_data.size(); ++i) {
        double residual = y_data[i] - y_pred[i];
        std::cout << x_data[i] << "\t" << y_data[i] << "\t" 
                  << y_pred[i] << "\t" << residual << std::endl;
    }

    // Opsi untuk menyimpan hasil ke file CSV
    char save_option;
    std::cout << "\nSimpan hasil ke file CSV? (y/n): ";
    std::cin >> save_option;
    
    if (save_option == 'y' || save_option == 'Y') {
        saveResultsToCSV(x_data, y_data, y_pred, "hasil_regresi.csv");
    }

    return 0;
}