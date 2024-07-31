#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include "candidate_search.h"

using namespace cv;
using namespace std;


// **** �ߗ׉�f�Q�ineighbor�j�̑傫��
//   nbr >= 1
// �ߗ׉�f�Q�̈�ӂ� 2 * nbr + 1 �ƂȂ�
int TS_nbr = 4;

// �e�N�X�`���t�H���_�ƃt�@�C����
char file[512];
char TS_folder[] = "./textures/";
char TS_infile[] = "texture_o_icon.jpg";
char TS_outfile[] = "output_texture.jpg";

// �𑜓x
int TS_in_x;
int TS_in_y;
int TS_out_x = 256;
int TS_out_y = 256;

// �������̂��߂̗����̎�
unsigned int TS_out_seed = 1;

int main() {
    // ++++ �v�Z���Ԃ̌v��
    clock_t time_start, time_end;
    double  time_sec;
    int     time_m;
    double  time_s;

    // ���̓e�N�X�`��
    Mat intex;
    // �o�̓e�N�X�`��
    Mat outtex;

    /* ++++ �v�Z���Ԃ̌v���J�n */
    cout << "main() ----> start" << endl;
    time_start = clock();

    // ���̓e�N�X�`���̓ǂݍ���
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

    // CandidateSearch �N���X�̃C���X�^���X�쐬
    CandidateSearch candidate_search(TS_nbr, TS_in_x, TS_in_y, intex, TS_out_seed);

    // �e�N�X�`�������̎��s
    outtex = candidate_search.performSearch(TS_out_x, TS_out_y);

    // �o�̓e�N�X�`���̕ۑ��ƕ\��
    strcpy_s(file, sizeof(file), TS_folder);
    strcat_s(file, sizeof(file), TS_outfile);
    imwrite(file, outtex);
    namedWindow("output", WINDOW_AUTOSIZE);
    imshow("output", outtex);

    /* ++++ �v�Z���Ԃ̌v���I�� */
    time_end = clock();
    time_sec = (double)(time_end - time_start) / (double)CLOCKS_PER_SEC;
    time_m = (int)(time_sec / 60.0);
    time_s = time_sec - (double)time_m * 60.0;
    cout << "main() : time >> " << time_sec << " sec. = " << time_m << " min. " << time_s << " sec." << endl;
    cout << "main() <---- end" << endl;

    // �L�[���͑҂�
    waitKey(0);

    return 0;
}
