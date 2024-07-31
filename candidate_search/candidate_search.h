#ifndef CANDIDATESEARCH_H
#define CANDIDATESEARCH_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <random>

using namespace cv;
using namespace std;

class CandidateSearch {
public:
    // �R���X�g���N�^
    CandidateSearch(int nbr, int in_x, int in_y, const Mat& intex, unsigned int seed);

    // ���C������
    Mat performSearch(int out_x, int out_y);

private:
    int nbr;                    // �ߗ׉�f�Q�̃T�C�Y
    int in_x;                   // ���̓e�N�X�`���̕�
    int in_y;                   // ���̓e�N�X�`���̍���
    const Mat& intex;           // ���̓e�N�X�`��
    unsigned int seed;          // �����̎�
    default_random_engine random_value; // �����G���W��
    uniform_int_distribution<int> dist_x; // X���W�̗������z
    uniform_int_distribution<int> dist_y; // Y���W�̗������z

    Mat outtex;                // �o�̓e�N�X�`��
    Mat coordinate_map;        // ���W�L���p�}�b�v

    // �o�̓e�N�X�`���ƍ��W�}�b�v�̏�����
    void initializeTextures(int out_x, int out_y);

    // ���s�N�Z��
    vector<vector<int>> generateCandidates(int oy, int ox, int out_x, int out_y);

    // SSD�iSum of Squared Differences�j�̌v�Z
    int calculateSSD(int oy, int ox, const vector<int>& candidate, int out_x, int out_y);

    // �o�̓e�N�X�`���̍X�V
    void updateOutputTexture(int oy, int ox, int ix_s, int iy_s);
};

#endif // CANDIDATESEARCH_H
