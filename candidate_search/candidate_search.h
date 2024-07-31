#ifndef CANDIDATESEARCH_H
#define CANDIDATESEARCH_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <random>

using namespace cv;
using namespace std;

class CandidateSearch {
public:
    // コンストラクタ
    CandidateSearch(int nbr, int in_x, int in_y, const Mat& intex, unsigned int seed);

    // メイン処理
    Mat performSearch(int out_x, int out_y);

private:
    int nbr;                    // 近隣画素群のサイズ
    int in_x;                   // 入力テクスチャの幅
    int in_y;                   // 入力テクスチャの高さ
    const Mat& intex;           // 入力テクスチャ
    unsigned int seed;          // 乱数の種
    default_random_engine random_value; // 乱数エンジン
    uniform_int_distribution<int> dist_x; // X座標の乱数分布
    uniform_int_distribution<int> dist_y; // Y座標の乱数分布

    Mat outtex;                // 出力テクスチャ
    Mat coordinate_map;        // 座標記憶用マップ

    // 出力テクスチャと座標マップの初期化
    void initializeTextures(int out_x, int out_y);

    // 候補ピクセル
    vector<vector<int>> generateCandidates(int oy, int ox, int out_x, int out_y);

    // SSD（Sum of Squared Differences）の計算
    int calculateSSD(int oy, int ox, const vector<int>& candidate, int out_x, int out_y);

    // 出力テクスチャの更新
    void updateOutputTexture(int oy, int ox, int ix_s, int iy_s);
};

#endif // CANDIDATESEARCH_H
