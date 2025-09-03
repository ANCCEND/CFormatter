#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct score
{
    char uid[11];
    char name[10];
    int english;
    int math;
    int physics;
    int c;
    float average;
    struct score *next;
} Score;

int num;

void input(Score *);
void output(Score *);
void change(Score *);
void Average(Score *);
void putAverage(Score *head);
void putAll(Score *);
Score *mergeSort(Score *head);
Score *split(Score *head);
Score *merge(Score *left, Score *right);
void inputFile(Score *head);
Score *readFile(void);

int main(void)
{
    int ctr;
    Score *head = malloc(sizeof(Score));
    head->next = NULL;
    void (*p[3])(Score *) = {input, output, change};
    while (scanf("%d", &ctr) == 1)
    {
        if (ctr == 0)
            break;
        else if (ctr <= 3)
        {
            (*p[ctr - 1])(head);
        }
        else if (ctr == 4)
        {
            Average(head);
            putAverage(head);
        }
        else if (ctr == 5)
        {
            Average(head);
            putAll(head);
        }
        else if (ctr == 6)
        {
            inputFile(head);
            Score *x = head, *q = x->next;
            for (int i = 0; i < num; i++)
            {
                free(x);
                x = q;
                if (q->next != NULL)
                {
                    q = q->next;
                }
            }
        }
        else
            head = readFile();
    }
    Score *x = head, *q = x->next;
    for (int i = 0; i < num; i++)
    {
        free(x);
        x = q;
        if (q->next != NULL)
        {
            q = q->next;
        }
    }

    return 0;
}

void input(Score *head)
{
    int temp;
    scanf("%d", &temp);
    Score *p = head;
    if (num != 0)
    {
        for (int i = 0; i < num; i++)
        {
            p = p->next;
        }
    }
    for (int i = 0; i < temp; i++)
    {
        Score *node = malloc(sizeof(Score));
        node->next = NULL;
        p->next = node;
        p = p->next;
        scanf("%s %s %d %d %d %d", &p->uid, &p->name, &p->english, &p->math, &p->physics, &p->c);
    }
    num += temp;
}

void output(Score *head)
{
    Score *p = head->next;
    for (int i = 0; i < num; i++)
    {
        printf("%s %s %d %d %d %d\n", p->uid, p->name, p->english, p->math, p->physics, p->c);
        p = p->next;
    }
}

void change(Score *head)
{
    Score *p = head->next;
    char uid[11];
    int n;
    scanf("%s %d", uid, &n);
    for (int i = 0; i < num; i++)
    {
        if (strcmp(uid, p->uid) == 0)
        {
            switch (n)
            {
            case 1:
                scanf("%d", &p->english);
                break;
            case 2:
                scanf("%d", &p->math);
                break;
            case 3:
                scanf("%d", &p->physics);
                break;
            case 4:
                scanf("%d", &p->c);
                break;
            }
            break;
        }
        p = p->next;
    }
}

void putAverage(Score *head)
{
    Score *p = head->next;
    for (int i = 0; i < num; i++)
    {
        printf("%s %s %.2f\n", p->uid, p->name, p->average);
        p = p->next;
    }
}

void Average(Score *head)
{
    Score *p = head->next;
    for (int i = 0; i < num; i++)
    {
        p->average = (float)(p->english + p->math + p->physics + p->c) / 4;
        p = p->next;
    }
    head->next = mergeSort(head->next);
}

void putAll(Score *head)
{
    Score *p = head->next;
    for (int i = 0; i < num; i++)
    {
        printf("%s %s %d %.2f\n", p->uid, p->name, p->english + p->math + p->physics + p->c, p->average);
        p = p->next;
    }
}

Score *mergeSort(Score *head)
{
    if (head == NULL || head->next == NULL)
    {
        return head;
    }
    Score *mid = split(head);
    Score *left = mergeSort(head);
    Score *right = mergeSort(mid);
    return merge(left, right);
}

Score *split(Score *head)
{
    Score *fast = head->next, *slow = head;
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    Score *mid = slow->next;
    slow->next = NULL;
    return mid;
}

Score *merge(Score *left, Score *right)
{
    Score dummy, *p = &dummy;
    dummy.next = NULL;
    while (left != NULL && right != NULL)
    {
        if (left->average < right->average)
        {
            p->next = left;
            left = left->next;
        }
        else
        {
            p->next = right;
            right = right->next;
        }
        p = p->next;
    }
    p->next = (left != NULL) ? left : right;
    return dummy.next;
}

void inputFile(Score *head)
{
    Score *p = head->next;
    FILE *file = fopen("stu.dat", "wb");
    if (file == NULL)
    {
        perror("文件打开错误");
        exit(EXIT_FAILURE);
    }
    while (p != NULL)
    {
        fwrite(p, sizeof(Score), 1, file);
        p = p->next;
    }
    fclose(file);
}

Score *readFile(void)
{
    FILE *file = fopen("stu.dat", "rb");
    if (file == NULL)
    {
        perror("文件打开错误");
        exit(EXIT_FAILURE);
    }
    Score *head = malloc(sizeof(Score)), *p = malloc(sizeof(Score));
    head->next = p, p->next = NULL;
    Score *temp = p;
    while (fread(p, sizeof(Score), 1, file))
    {
        Score *q = malloc(sizeof(Score));
        p->next = q, q->next = NULL;
        temp = p;
        p = q;
    }
    free(p);
    temp->next = NULL;
    return head;
}