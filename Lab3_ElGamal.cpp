#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int p = 883;
int g = 39;

char alphabet[60] = { 'а','б','в','г','д','е','ж','з','и','к','л','м','н','о','п','р','с','т','у','ф','х','ц','ч','ш','щ','ъ','ы','ь','э','ю','я',
                      'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' ',',','.' };


int stepen(int s, int y, int r)  // Возведение в степень по модулю (a = g^k mod p)
{
    if (y == 0) return 1;
    int z = stepen(s, y / 2, r);
    if (y % 2 == 0)
        return (z * z) % r;
    else
        return (s * z * z) % r;
}

int stepenb(int s, int m, int y, int r) // Возведение в степень по модулю для числа b (b = y^k M mod p)
{
    if (y == 0) return 1;
    int z = stepen(s, y, r);
    return z * m % r;
}

void Cipher()   // Шифровка сообщения  
{
    ifstream Input("Input.txt");
    if (!Input.is_open()) {
        std::cout << " Ошибка открытия файла" << endl;
        exit(0);
    }

    ofstream Output("Output.txt");
    if (!Output.is_open())
    {
        std::cout << " Ошибка открытия файла" << endl;
        exit(0);
    }

    string msg = "", buf = "";
    while (!Input.eof())
    {
        Input >> buf;
        msg += buf;
    }
    cout << "\n Исходное сообщение: " << msg;

    srand(time(NULL));
    int x = rand() % (p - 2) + 2; // Закрытый ключ
    srand(time(NULL));
    int k = rand() % (p - 3) + 2; // Сессионный ключ
    cout << "\n x = " << x << "\n k = " << k << endl;

    int y = stepen(g, x, p);
    int len = msg.size();
    int dec[60];
    int* A = new int[len * 2];
    int* B = new int[len * 2];

    cout << "\n Открытый ключ: (" << y << "," << g << "," << p << ")";
    Output << x << endl << y << endl;

    cout << "\n\n Закодированное сообщение: ";
    
    string inmsg = "", m = "";
    for (int i = 0; i < len ; i++)
    {
        for (int j = 0; j < 60; j++)
        {
            if (msg.c_str()[i] == alphabet[j])
            {
                inmsg += to_string(j + 10);
            }
        }
    }
    cout << inmsg << endl;
    int l = inmsg.length();
    int i = 0, count = 0;

    while ((i < l - 2))
    {
        m = inmsg.substr(i, 3);
        if (stoi(m) < p && inmsg[i + 3] != '0')
        {
            dec[count] = stoi(m);
            count ++;
            i += 3;
        }
        else
        {
            m = inmsg.substr(i, 2);
            dec[count] = stoi(m);
            count ++;
            i += 2;
        }
    }

    m = inmsg.substr(i, 3);
    if (stoi(m) < p)
    {
        dec[count] = stoi(m);
        count++;
        i += 3;
    }
    else
    {
        m = inmsg.substr(i, 2);
        dec[count] = stoi(m);
        count++;
        i += 2;
    }

    cout << "\n Зашифрованное сообщение:";
    for (int i = 0; i < count; i++)
    {
        A[i] = stepen(g, k, p);
        B[i] = stepenb(y, dec[i], k, p);
        cout << " " << A[i] << " " << B[i];
        Output << " " << A[i] << " " << B[i];
    }
    cout << endl;
}

void DeCipher() // Дешифровка сообщения
{
    int x, y, i = 0, mout = 0;
    int* A = new int[60];
    string outmsg = "", msg = "";

    ifstream Output("Output.txt");
    if (!Output.is_open()) {
        std::cout << " Ошибка открытия файла" << endl;
        exit(0);
    }

    Output >> x;
    Output >> y;
    cout << "\n Открытый ключ: (" << y << "," << g << "," << p << ")";
    cout << "\n Закрытый ключ: (" << x << ")";

    cout << "\n\n Исходное зашифрованное сообщение: ";

    while (!Output.eof())
    {
        Output >> A[i];
        cout << A[i] << " ";
        i++;
    }

    cout << "\n\n Дешифрованное сообщение: ";

    int len = i;

    for (int i = 0; i < len - 1; i += 2)
    {
        mout = stepenb(A[i], A[i + 1], p - 1 - x, p);
        outmsg += to_string(mout);
    }
    cout << outmsg;

    cout << "\n\n Декодированное сообщение: ";

    int l = outmsg.length();
    int* num = new int[l];

    for (int i = 0; i < l / 2; i++)
    {
        num[i] = stoi(outmsg.substr(i * 2, 2));

        for (int j = 0; j < 60; j++)
        {
            if (num[i] == j + 10)
            {
                msg += alphabet[j];
            }
        }
    }
    cout << msg << endl;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    int q;

    cout << "\n 1) Шифровка\n 2) Дешифровка\n Выберите действие: ";
    cin >> q;

    switch (q)
    {
        case 1:
        {
            Cipher();  // Шифровка сообщения
            break;
        }

        case 2:
        {
            DeCipher();  // Дешифровка сообщения
            break;
        }

        default:
        {
            cout << " Ошибка: Неверное действие";
            break;
        }
    }
    return 0;
}