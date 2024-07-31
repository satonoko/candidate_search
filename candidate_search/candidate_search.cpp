#include "candidate_search.h"

// コンストラクタ
CandidateSearch::CandidateSearch(int nbr, int in_x, int in_y, const Mat& intex, unsigned int seed)
    : nbr(nbr), in_x(in_x), in_y(in_y), intex(intex), seed(seed) {
    random_value.seed(seed);
    dist_x = uniform_int_distribution<int>(0, in_x - 1);
    dist_y = uniform_int_distribution<int>(0, in_y - 1);
}

// メイン処理
Mat CandidateSearch::performSearch(int out_x, int out_y) {
    outtex = Mat(out_y, out_x, CV_8UC3);          // 出力テクスチャ
    coordinate_map = Mat(out_y, out_x, CV_64FC2); // 座標記憶用マップ

    initializeTextures(out_x, out_y);

    int pxn = ((2 * nbr + 1) * (2 * nbr + 1)) / 2;
    int ssd_max = 3 * (255 * 255) * pxn;
    cout << "candidate_search() : nbr = " << nbr << ", pxn = " << pxn << endl;

    // 各画素について処理
    for (int oy = 0; oy < out_y; oy++) {
        for (int ox = 0; ox < out_x; ox++) {
            int ssd_min = ssd_max;
            int ix_s = -1;
            int iy_s = -1;

            vector<vector<int>> candidates = generateCandidates(oy, ox, out_x, out_y);

            // 近隣画素群の SSD 値の計算
            for (const auto& candidate : candidates) {
                int ssd = calculateSSD(oy, ox, candidate, out_x, out_y);
                if (ssd_min > ssd) {
                    ssd_min = ssd;
                    ix_s = candidate[0];
                    iy_s = candidate[1];
                    coordinate_map.at<Vec2d>(oy, ox)[0] = ix_s;
                    coordinate_map.at<Vec2d>(oy, ox)[1] = iy_s;
                }
            }

            updateOutputTexture(oy, ox, ix_s, iy_s);
        }
    }

    return outtex;
}

// 出力テクスチャと座標マップの初期化
void CandidateSearch::initializeTextures(int out_x, int out_y) {
    for (int oy = 0; oy < out_y; oy++) {
        for (int ox = 0; ox < out_x; ox++) {
            coordinate_map.at<Vec2d>(oy, ox)[0] = dist_x(random_value);
            coordinate_map.at<Vec2d>(oy, ox)[1] = dist_y(random_value);
            for (int c = 0; c < 3; c++) {
                outtex.at<Vec3b>(oy, ox)[c] = intex.at<Vec3b>(
                    coordinate_map.at<Vec2d>(oy, ox)[1],
                    coordinate_map.at<Vec2d>(oy, ox)[0])[c];
            }
        }
    }
}

// 候補ピクセル
vector<vector<int>> CandidateSearch::generateCandidates(int oy, int ox, int out_x, int out_y) {
    vector<vector<int>> candidates;     //候補ピクセルを格納するベクトル
    for (int ny = -nbr; ny <= nbr; ny++) {
        for (int nx = -nbr; nx <= nbr; nx++) {
            vector<int> candidate(2);   //候補ピクセル
            candidate[0] = coordinate_map.at<Vec2d>((oy + ny + out_y) % out_y, (ox + nx + out_x) % out_x)[0] - nx;
            candidate[1] = coordinate_map.at<Vec2d>((oy + ny + out_y) % out_y, (ox + nx + out_x) % out_x)[1] - ny;

            if (candidate[0] + nbr >= in_x || candidate[0] - nbr < 0 ||
                candidate[1] + nbr >= in_y || candidate[1] - nbr < 0) {
                candidate[0] = dist_x(random_value);
                candidate[1] = dist_y(random_value);
            }

            //候補ピクセルに追加
            candidates.push_back(candidate);
        }
    }
    return candidates;
}

// SSD（Sum of Squared Differences）の計算
int CandidateSearch::calculateSSD(int oy, int ox, const vector<int>& candidate, int out_x, int out_y)
{
    int ssd = 0;
    for (int ny = -nbr; ny <= 0; ny++) {
        for (int nx = -nbr; nx <= nbr; nx++) {
            if (ny == 0 && nx == 0) continue;
            for (int c = 0; c < 3; c++) {
                int s = outtex.at<Vec3b>((oy + ny + out_y) % out_y, (ox + nx + out_x) % out_x)[c]
                    - intex.at<Vec3b>((candidate[1] + ny + in_y) % in_y, (candidate[0] + nx + in_x) % in_x)[c];
                ssd += (s * s);
            }
        }
    }
    return ssd;
}

// 出力テクスチャの更新
void CandidateSearch::updateOutputTexture(int oy, int ox, int ix_s, int iy_s) {
    for (int c = 0; c < 3; c++) {
        outtex.at<Vec3b>(oy, ox)[c] = intex.at<Vec3b>(iy_s, ix_s)[c];
    }
}
