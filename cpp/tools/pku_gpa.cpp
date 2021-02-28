// 北大 GPA 计算公式

double GPA(int score)
{
    if (score < 0 || score > 100)
        return -1;
    else if (score < 60)
        return 0;
    return 4 - 3 * (100 - score) * (100 - score) / 1600.;
}