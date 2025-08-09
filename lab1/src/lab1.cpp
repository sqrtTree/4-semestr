#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <unordered_map>

#include <stdio.h>
using namespace std;

template <class T>
class Sort
{
    public:
    static int bubble_sort(T *A, int n, int *ind)
    {
        int i, m, count = 0;
        for(i = 0; i < n; i++) ind[i] = i;

        for (m = n; m > 1; m--)
            for (i = 0; i < m-1; i++)
            {
                if (A[ind[i]] > A[ind[i+1]]) 
                    swap(ind[i], ind[i+1]);
                count++;
            }
        // cout << Sort<T>::check(A, ind, n);
        // for(i = 0; i < n; i++) cout << A[ind[i]] << '|';
        return count;
    }

    static void merge(T *A, int b, int c, int e, int *D, int *ind)
    {
        int i = b, j = c+1;
        for (; b <= e; b++)
            if (j > e) D[b] = ind[i++];
            else if (i > c) D[b] = ind[j++];    
            else if (A[ind[j]] > A[ind[i]]) D[b]=ind[i++];
            else D[b] = ind[j++];
    }

    static int merge_sort(T *A, int n, int *ind)
    {
        int s, b, c, e, count = 0, *D = new int[n];
        for(int i = 0; i < n; i++) ind[i] = i;

        for (s = 1; s < n; s *= 2)
        {
            for (b = 0; b < n; b += s*2)
            {
                c = min(b+s-1, n-1);
                e = min(c+s, n-1);
                merge(A, b, c, e, D, ind);
            }
            for (b = 0; b < n; b++) ind[b] = D[b];
        } 
        // for(int i = 0; i < n; i++) cout << A[ind[i]] << ' ';
        delete []D;
        return 1;
    }

    static int shell_sort(T *A, int n, int *ind)
    {
        int i, j, h, count = 0;
        for(i = 0; i < n; i++) ind[i] = i;

        for(h = 1; h <= n/9; h = h*3+1);
        while(h >= 1)
        {
            for(i = h; i < n; i++)
            {
                for(j = i-h; j >= 0 && A[ind[j]] > A[ind[j+h]]; j -= h)
                {
                    swap(ind[j], ind[j + h]);
                    count++;
                }
            }
            h = (h - 1) / 3;
        }
        // for(i = 0; i < n; i++) cout << A[ind[i]] << ' ';
        return count;
    }

    static bool check(T *A, int *ind, int n)
    {
        for(int i = 0; i < n-1; i++)
            if(A[ind[i]] > A[ind[i + 1]]) return false;

        return true;
    }
};

class Participant
{
    public:
    static int count;
    unordered_map<string , int> m{{"id", 0}, {"looses", 0}, {"spend_time", 0}, {"solved_case", 0}, {"score", 0}};

    Participant()
    {
        m["score"] = rand()%26;//0-25
        m["solved_case"] = rand()%6;//0-5
        m["spend_time"] = rand()%241 + 60;//60-300
        m["looses"] = rand()%21;//0-20
        count++;
        m["id"] = count;
    }
    bool operator>(Participant &obj)
    {
        if(m["score"] > obj.m["score"]) return false;
        if(m["score"] < obj.m["score"]) return true;

        if(m["solved_case"] > obj.m["solved_case"]) return false;
        if(m["solved_case"] < obj.m["solved_case"]) return true;
        
        if(m["spend_time"] > obj.m["spend_time"]) return true;
        if(m["spend_time"] < obj.m["spend_time"]) return false;
        
        if(m["looses"] > obj.m["looses"]) return true;
        if(m["looses"] < obj.m["looses"]) return false;
        
        if(m["id"] > obj.m["id"]) return true;
        return false;
    };
    friend ostream& operator<<(ostream &stream, Participant &obj)
    {
        for(const auto n: obj.m)
            stream << setw(11) << n.first << " —" << setw(4) << n.second << " |";
        stream << endl;
        return stream;
    };
};
int Participant::count = 0;

int main()
{
    srand(time(NULL));
    ofstream MyFile("output.txt");
    int n = 20;
    int output_n = 20;
    int *array = new int[n], *ind = new int[n];

    for(int i = 0; i < n; i++)
        array[i] = (rand()%1001);

    Participant *par_arr = new Participant[n];

    Sort<int>::bubble_sort(array, n, ind);
    Sort<Participant>::bubble_sort(par_arr, n, ind);
    MyFile << setw(40) << "BUBBLE_SORT" << endl;
    for(int i = 0; i < output_n; i++)
        MyFile << par_arr[ind[i]];

    Sort<int>::shell_sort(array, n, ind);
    Sort<Participant>::shell_sort(par_arr, n, ind);
    MyFile << setw(40) << "SHELL_SORT" << endl;
    for(int i = 0; i < output_n; i++)
        MyFile << par_arr[ind[i]];

    Sort<int>::merge_sort(array, n, ind);
    Sort<Participant>::merge_sort(par_arr, n, ind);
    MyFile << setw(40) << "MERGE_SORT" << endl;
    for(int i = 0; i < output_n; i++)
        MyFile << par_arr[ind[i]];


    delete []array;
    delete []par_arr;
    delete []ind;
    MyFile.close();
    return 0;
};




// cout << boost::format("\"{\"GPU\": [%1%], \"System\": [%2%], \"CPU\": [%3%], \"HDD\": [%4%]}\"") % temp[0] % temp[1] % temp[2] % temp[3];