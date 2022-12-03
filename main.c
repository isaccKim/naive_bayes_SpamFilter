#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
typedef struct s_mail
{
    char word[1024];
    int count;
    double percent;
    int label;
} Mail;

void labeling(Mail *temp, int size, int check)
{

    int cnt = 0;
    if (check == 0)
    {
        for (int i = 5; i < size; i++)
        {
            if (strcmp(temp[i].word, "spam\n") == 0 && strcmp(temp[i + 1].word, "Subject\n") == 0)
            {
                cnt++;
            }
            temp[i].label = cnt;
        }
    }
    else if (check == 1)
    {
        for (int i = 5; i < size; i++)
        {
            if (strcmp(temp[i].word, "ham\n") == 0 && strcmp(temp[i + 1].word, "Subject\n") == 0)
            {
                cnt++;
            }
            temp[i].label = cnt;
        }
    }
}

void trainer(char readFile[100], char writeTxt[100])
{

    char str_tmp[1024];
    int cc = 0;

    FILE *pFile = NULL;
    int scnt = 0;

    pFile = fopen(readFile, "r");
    FILE *fp = fopen(writeTxt, "w"); // test파일을 w(쓰기) 모드로 열기
    if (pFile != NULL)
    {
        while (!feof(pFile))
        {
            char temp[1024];
            char str[1024];

            for (int i = 0; i < 1024; i++)
            {
                temp[i] = ' ';
                str[i] = ' ';
            }

            scnt = 0;
            fgets(str, 1024, pFile);
            for (int i = 0; i < sizeof(str); i++)
            {
                if (isalpha(str[i]) == 1)
                {
                    temp[scnt] = str[i];
                    scnt++;
                    if (isalpha(str[i + 1]) == 0)
                    {
                        temp[scnt] = ' ';
                        scnt++;
                    }
                    else if (str[i + 1] == ' ')
                    {
                        temp[scnt] = ' ';
                        scnt++;
                    }
                }
                else
                {
                    continue;
                }
            }
            char *num;
            num = strtok(temp, " ");
            int count = 0;

            while (num != NULL)
            {
                if (isalpha(num[0]))
                {
                    fputs(num, fp);  //문자열 입력
                    fputs("\n", fp); //문자열 입력
                }

                num = strtok(NULL, " ");
            }
        }
    }
    fclose(pFile);
    fclose(fp);
}

int calMail(Mail *mail, int type)
{
    const int max = 1024;
    char line[max];
    int scnt = 0;
    double sPercent = 0.0;
    int tcnt = 0;

    FILE *in;

    if (type == 0)
    {
        in = fopen("spamTrain.txt", "r");
    }

    if (type == 1)
    {
        in = fopen("hamTrain.txt", "r");
    }

    while (!feof(in))
    {
        fgets(line, max, in);
        strcpy(mail[scnt].word, line);

        scnt++;
    }

    strcpy(mail[scnt - 1].word, " ");

    labeling(mail, scnt, type);

    int flag = 0;

    int label = -1;

    for (int i = 3; i < scnt; i++) // i=3, format : num label text <= delete
    {
        tcnt = 0;

        for (int j = 3; j < scnt; j++) // i=3, format : num label text <= delete
        {
            if (label == mail[j].label)
                continue;
            if (strcmp(mail[j].word, "spam\n") == 0 && strcmp(mail[j + 1].word, "Subject\n") == 0)
                continue;
            if (strcmp(mail[j - 1].word, "spam\n") == 0 && strcmp(mail[j].word, "Subject\n") == 0)
                continue;
            if (strcmp(mail[j].word, "ham\n") == 0 && strcmp(mail[j + 1].word, "Subject\n") == 0)
                continue;
            if (strcmp(mail[j - 1].word, "ham\n") == 0 && strcmp(mail[j].word, "Subject\n") == 0)
                continue;

            if (strcmp(mail[i].word, mail[j].word) == 0)
            {
                label = mail[j].label;
                tcnt++;
            }
        }

        if (tcnt == 0)
            tcnt = 1;
        mail[i].count = tcnt;
        mail[i].percent = (double)tcnt / 100;
    }

    fclose(in);

    return scnt;
}

void testMail(double *spamPercent, Mail *spam_mail, Mail *ham_mail, int type, int s_size, int h_size, char test[50])
{
    if (type == 0)
    {
        trainer(test, "spam.txt");
    }
    if (type == 1)
    {
        trainer(test, "ham.txt");
    }

    const int max = 1024;
    char line[max];
    int cnt = 0;
    int tcnt = 0;
    Mail *temp = malloc(sizeof(Mail) * 50000);
    FILE *in;

    if (type == 0)
    {
        in = fopen("spam.txt", "r");
    }
    if (type == 1)
    {
        in = fopen("ham.txt", "r");
    }

    for (int i = 0; i < 20; i++)
    {
        spamPercent[i] = 1.0;
    }
    while (!feof(in))
    {
        fgets(line, max, in);
        strcpy(temp[tcnt].word, line);
        tcnt++;
    }

    int pcount = 0;
    double spercent = 1;
    double hpercent = 1;

    for (int i = 5; i < tcnt; i++) // i = 5 , 첫번째 'spam Subject'는 건너뛰기
    {
        if (type == 0)
        {
            if (strcmp(temp[i].word, "spam\n") == 0 && strcmp(temp[i + 1].word, "Subject\n") == 0)
            {
                spamPercent[pcount] = spercent / (spercent + hpercent); // 라벨단어 spam에는 0.6 값이 들어간다. 이는 연산에 사용되지 않는다. 메일 내용에서 spam 단어가 나오는 횟수에 대한 확률을 임시로 넣어두었다.
                pcount++;
                spercent = 1;
                hpercent = 1;
                i += 1;
                continue; // spam Subject 연달아 나올경우 건너뛰기
            }
        }
        if (type == 1)
        {
            if (strcmp(temp[i].word, "ham\n") == 0 && strcmp(temp[i + 1].word, "Subject\n") == 0)
            {
                spamPercent[pcount] = spercent / (spercent + hpercent);
                pcount++;
                spercent = 1;
                hpercent = 1;
                i += 1;
                continue; // ham Subject 연달아 나올경우 건너뛰기
            }
        }

        int flag = 0;
        double ts_percent = 1;
        double th_percent = 1;

        for (int j = 0; j < s_size; j++)
        {
            if (strcmp(temp[i].word, spam_mail[j].word) == 0)
            {
                if (spam_mail[j].percent != 0)
                {
                    ts_percent = spam_mail[j].percent;
                    flag = 1;
                    break;
                }
            }
        }
        if (flag == 1)
        {
            for (int j = 0; j < h_size; j++)
            {
                if (strcmp(temp[i].word, ham_mail[j].word) == 0)
                {
                    if (ham_mail[j].percent != 0)
                    {
                        th_percent = ham_mail[j].percent;
                        flag = 2;
                        break;
                    }
                }
            }
        }

        if (flag == 2)
        {
            spercent *= ts_percent;
            hpercent *= th_percent;
        }

        if (type == 0)
        {
            if (i == tcnt - 1)
            {
                spamPercent[pcount] = spercent / (spercent + hpercent);
                pcount++;
                spercent = 1;
                hpercent = 1;
            }
        }
        if (type == 1)
        {
            if (i == tcnt - 1)
            {
                spamPercent[pcount] = spercent / (spercent + hpercent);
                pcount++;
                spercent = 1;
                hpercent = 1;
            }
        }
    }
    fclose(in);
    free(temp);
}

int calAccuracy(double *test, float threshold, int type)
{
    int cnt = 0;
    if (type == 0)
    {
        printf("\n ____Spam____ \n");
        for (int i = 0; i < 20; i++)
        {

            if (test[i] > threshold)
            {
                printf("(%d) %g : %s\n", i + 1, test[i], "spam");
                cnt++;
            }
            else
            {
                printf("(%d) %g : %s\n", i + 1, test[i], "non Spam");
            }
        }
    }

    if (type == 1)
    {
        printf("\n ____non_Spam____ \n");
        for (int i = 0; i < 20; i++)
        {

            if (test[i] < threshold)
            {
                printf("(%d) %g : %s\n", i + 1, test[i], "non Spam");
                cnt++;
            }
            else
            {
                printf("(%d) %g : %s\n", i + 1, test[i], "Spam");
            }
        }
    }

    return cnt;
}

int main(int argc, char *argv[])
{
    Mail *spam_mail = malloc(sizeof(Mail) * 50000);
    Mail *ham_mail = malloc(sizeof(Mail) * 50000);
    double *sresult = malloc(sizeof(double) * 20);
    double *hresult = malloc(sizeof(double) * 20);
    int s_size;
    int h_size;
    int ac_cnt = 0;
    double accuracy = 0;
    float threshold = 0.6; //  < ======= theshold 바꾸기

    trainer("csv/train/dataset_spam_train100.csv", "spamTrain.txt");
    trainer("csv/train/dataset_ham_train100.csv", "hamTrain.txt");
    s_size = calMail(spam_mail, 0);
    h_size = calMail(ham_mail, 1);
    testMail(sresult, spam_mail, ham_mail, 0, s_size, h_size, "csv/test/dataset_spam_test20.csv");
    testMail(hresult, spam_mail, ham_mail, 1, s_size, h_size, "csv/test/dataset_ham_test20.csv");
    ac_cnt += calAccuracy(sresult, threshold, 0);
    int temp = ac_cnt;
    printf("\n======> Correct : %d\n\n", ac_cnt);
    ac_cnt += calAccuracy(hresult, threshold, 1);
    printf("\n======> Correct : %d\n", ac_cnt - temp);
    accuracy = ac_cnt / 40.0 * 100;
    printf("\n=====================\n");
    printf("Threshhold : %.1f accuracy : %.4f", threshold, accuracy);
    printf("\n=====================\n");
    return 0;
}