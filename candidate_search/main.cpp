#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include "candidate_search.h"

using namespace cv;
using namespace std;


// **** 近隣画素群（neighbor）の大きさ
//   nbr >= 1
// 近隣画素群の一辺が 2 * nbr + 1 となる
int TS_nbr = 4;

// テクスチャフォルダとファイル名
char file[512];
char TS_folder[] = "./textures/";
char TS_infile[] = "texture_o_icon.jpg";
char TS_outfile[] = "output_texture.jpg";

// 解像度
int TS_in_x;
int TS_in_y;
int TS_out_x = 256;
int TS_out_y = 256;

// 初期化のための乱数の種
unsigned int TS_out_seed = 1;

int main() {
    // ++++ 計算時間の計測
    clock_t time_start, time_end;
    double  time_sec;
    int     time_m;
    double  time_s;

    // 入力テクスチャ
    Mat intex;
    // 出力テクスチャ
    Mat outtex;

    /* ++++ 計算時間の計測開始 */
    cout << "main() ----> start" << endl;
    time_start = clock();

    // 入力テクスチャの読み込み
    strcpy_s(file, sizeof(file), TS_folder);
    strcat_s(file, sizeof(file), TS_infile);
    intex = imread(file);
    TS_in_x = intex.cols;
    TS_in_y = intex.rows;

    if (intex.empty()) {
        cerr << "Error: Could not open or find the image!" << endl;
        return -1;
    }

    namedWindow("input", WINDOW_AUTOSIZE);
    imshow("input", intex);

    /* ++++ print */
    printf("main() : TS_infile >> %s\n", TS_infile);
    printf("intex_cv.cols, rows = %d, %d\n", intex.cols, intex.rows);

    // CandidateSearch クラスのインスタンス作成
    CandidateSearch candidate_search(TS_nbr, TS_in_x, TS_in_y, intex, TS_out_seed);

    // テクスチャ合成の実行
    outtex = candidate_search.performSearch(TS_out_x, TS_out_y);

    // 出力テクスチャの保存と表示
    strcpy_s(file, sizeof(file), TS_folder);
    strcat_s(file, sizeof(file), TS_outfile);
    imwrite(file, outtex);
    namedWindow("output", WINDOW_AUTOSIZE);
    imshow("output", outtex);

    /* ++++ 計算時間の計測終了 */
    time_end = clock();
    time_sec = (double)(time_end - time_start) / (double)CLOCKS_PER_SEC;
    time_m = (int)(time_sec / 60.0);
    time_s = time_sec - (double)time_m * 60.0;
    cout << "main() : time >> " << time_sec << " sec. = " << time_m << " min. " << time_s << " sec." << endl;
    cout << "main() <---- end" << endl;

    // キー入力待ち
    waitKey(0);

    return 0;
}
