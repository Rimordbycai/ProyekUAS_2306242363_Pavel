# Aplikasi Regresi Linier untuk Penentuan Parameter Model Kinetika Pertumbuhan Mikroba

**Nama:** Muhammad Pavel  
**NPM:** 2306242363

## Daftar Isi
- [Pendahuluan](#pendahuluan)
- [Teori Dasar](#teori-dasar)
  - [Regresi Linier](#regresi-linier)
  - [Model Kinetika Monod](#model-kinetika-monod)
  - [Linearisasi Model Monod](#linearisasi-model-monod)
- [Implementasi Program](#implementasi-program)
  - [Struktur Program](#struktur-program)
  - [Algoritma Regresi Linier](#algoritma-regresi-linier)
  - [Perhitungan Parameter Kinetika](#perhitungan-parameter-kinetika)
- [Contoh Penggunaan](#contoh-penggunaan)
- [Kesimpulan](#kesimpulan)

## Pendahuluan

Program ini merupakan implementasi dari metode numerik regresi linier yang diaplikasikan untuk menentukan parameter kinetika pertumbuhan mikroba berdasarkan model Monod. Dalam bidang bioteknologi dan mikrobiologi, model kinetika pertumbuhan mikroba sangat penting untuk memahami dinamika populasi mikroba, merancang bioreaktor, dan mengoptimalkan kondisi fermentasi.

## Teori Dasar

### Regresi Linier

Regresi linier merupakan metode statistik yang digunakan untuk memodelkan hubungan antara variabel dependen (y) dengan satu atau lebih variabel independen (x). Dalam kasus regresi linier sederhana, model matematisnya adalah:

```
y = a₀ + a₁x
```

Dimana:
- y adalah variabel dependen
- x adalah variabel independen
- a₀ adalah intercept (konstanta)
- a₁ adalah slope (gradien)

Untuk menentukan nilai a₀ dan a₁, metode yang digunakan adalah metode least squares (kuadrat terkecil), yang meminimalkan jumlah kuadrat dari residual (selisih antara nilai aktual dan nilai prediksi). Formula untuk menghitung a₀ dan a₁ adalah:

```
a₁ = (n∑xy - ∑x∑y) / (n∑x² - (∑x)²)
a₀ = ȳ - a₁x̄
```

Dimana:
- n adalah jumlah data
- ∑xy adalah jumlah hasil kali x dan y
- ∑x adalah jumlah nilai x
- ∑y adalah jumlah nilai y
- ∑x² adalah jumlah kuadrat nilai x
- x̄ adalah rata-rata nilai x
- ȳ adalah rata-rata nilai y

Kualitas model regresi dapat dinilai dengan koefisien determinasi (R²) yang menunjukkan proporsi variansi dalam variabel dependen yang dapat dijelaskan oleh variabel independen:

```
R² = SSR / SST
```

Dimana:
- SSR (Sum of Squares Regression) = ∑(ŷᵢ - ȳ)²
- SST (Total Sum of Squares) = ∑(yᵢ - ȳ)²
- ŷᵢ adalah nilai prediksi y untuk data ke-i

### Model Kinetika Monod

Model Monod adalah model matematis yang menggambarkan pertumbuhan mikroba sebagai fungsi dari konsentrasi substrat pembatas pertumbuhan. Persamaan Monod dinyatakan sebagai:

```
μ = μmax × S / (K + S)
```

Dimana:
- μ adalah laju pertumbuhan spesifik (day⁻¹)
- μmax adalah laju pertumbuhan spesifik maksimum (day⁻¹)
- S adalah konsentrasi substrat (mg/L)
- K adalah konstanta saturasi (mg/L), yaitu konsentrasi substrat saat laju pertumbuhan = ½μmax

### Linearisasi Model Monod

Persamaan Monod bersifat non-linear, sehingga perlu ditransformasi menjadi bentuk linier untuk dapat menggunakan metode regresi linier. Salah satu bentuk transformasi yang umum digunakan adalah Lineweaver-Burk:

```
1/μ = (K/μmax)(1/S) + 1/μmax
```

Dengan transformasi ini, persamaan Monod dapat ditulis dalam bentuk persamaan linier y = a₀ + a₁x, dimana:
- y = 1/μ
- x = 1/S
- a₀ = 1/μmax
- a₁ = K/μmax

Setelah mendapatkan nilai a₀ dan a₁ dari regresi linier, parameter kinetika dapat dihitung sebagai:
- μmax = 1/a₀
- K = a₁ × μmax

## Implementasi Program

### Struktur Program

Program ini terdiri dari beberapa komponen utama:
1. Struktur data `RegressionResult` untuk menyimpan hasil regresi
2. Fungsi `linearRegression` untuk melakukan perhitungan regresi linier
3. Fungsi `calculatePredictions` untuk menghitung nilai prediksi
4. Fungsi `saveResultsToCSV` untuk menyimpan hasil ke file
5. Fungsi `main` yang mengatur alur program

### Algoritma Regresi Linier

Algoritma regresi linier diimplementasikan pada fungsi `linearRegression` yang melakukan langkah-langkah berikut:

1. Menghitung jumlah x, y, xy, x², dan y²
2. Menghitung rata-rata x dan y
3. Menghitung slope (a₁) dan intercept (a₀) menggunakan rumus:
   - a₁ = (n∑xy - ∑x∑y) / (n∑x² - (∑x)²)
   - a₀ = ȳ - a₁x̄
4. Menghitung koefisien determinasi (R²) dan korelasi (r)

```cpp
double sum_x = 0.0, sum_y = 0.0;
double sum_xy = 0.0, sum_x2 = 0.0, sum_y2 = 0.0;

// Menghitung jumlah x, y, xy, x², y²
for (int i = 0; i < n; ++i) {
    sum_x += x[i];
    sum_y += y[i];
    sum_xy += x[i] * y[i];
    sum_x2 += x[i] * x[i];
    sum_y2 += y[i] * y[i];
}

double x_mean = sum_x / n;
double y_mean = sum_y / n;

// Menghitung a₁ (slope) dan a₀ (intercept)
double numerator = n * sum_xy - sum_x * sum_y;
double denominator = n * sum_x2 - sum_x * sum_x;

result.a1_slope = numerator / denominator;
result.a0_intercept = y_mean - result.a1_slope * x_mean;

// Menghitung koefisien determinasi (R²)
double SST = sum_y2 - (sum_y * sum_y / n); // Total sum of squares
double SSR = 0.0; // Regression sum of squares
for (int i = 0; i < n; ++i) {
    double y_pred = result.a0_intercept + result.a1_slope * x[i];
    SSR += pow(y_pred - y_mean, 2);
}
```

### Perhitungan Parameter Kinetika

Setelah mendapatkan hasil regresi, parameter kinetika dihitung sebagai berikut:
- k_max = 1 / a₀
- K = a₁ × k_max

Dimana:
- k_max adalah laju pertumbuhan spesifik maksimum (μmax)
- K adalah konstanta saturasi Monod
- a₀ adalah intercept dari hasil regresi linier
- a₁ adalah slope dari hasil regresi linier

## Contoh Penggunaan

Program menyediakan opsi menggunakan data sampel yang telah ditransformasi (1/f vs 1/k) atau memasukkan data sendiri. Data sampel sudah diatur untuk model Monod dengan pasangan nilai x (1/S) dan y (1/μ).

Setelah melakukan regresi, program akan menampilkan:
1. Persamaan regresi dalam bentuk y = a₀ + a₁x
2. Koefisien determinasi (R²) dan korelasi (r)
3. Parameter kinetika k_max dan K
4. Tabel perbandingan data aktual dan prediksi
5. Opsi untuk menyimpan hasil ke file CSV

## Kesimpulan

Program ini mendemonstrasikan aplikasi metode numerik regresi linier dalam menentukan parameter kinetika pertumbuhan mikroba. Melalui transformasi linearisasi model Monod, parameter k_max dan K dapat ditentukan dengan akurat, yang kemudian dapat digunakan untuk memodelkan dan memprediksi pertumbuhan mikroba dalam berbagai kondisi substrat.

Pendekatan ini sangat berguna dalam bioreaktor design, optimasi proses fermentasi, dan studi dinamika populasi mikroba dalam lingkungan yang terkontrol.
