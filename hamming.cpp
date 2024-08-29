#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
using namespace std;

string decToBinary(int n)
{
  string ans = "";
  int binaryNum[32];

  int i = 0;
  while (n > 0)
  {
    binaryNum[i] = n % 2;
    n = n / 2;
    i++;
  }

  for (int j = i - 1; j >= 0; j--)
  {
    ans += binaryNum[j] + 48;
  }
  return ans;
}

int evenParity(int n, vector<int> v)
{
  bool flag = true;
  int count = 0;
  int cnt = 0;
  for (int i = n; i < v.size(); i++)
  {
    cnt++;
    if (flag)
    {
      if (v[i] == 1)
      {
        count++;
      }
    }
    if (cnt == n)
    {
      flag = !flag;
      cnt = 0;
    }
  }

  if (count % 2 == 0)
  {
    return 0;
  }
  return 1;
}

int oddParity(int n, vector<int> v)
{
  bool flag = true;
  int count = 0;
  int cnt = 0;
  for (int i = n; i < v.size(); i++)
  {
    cnt++;
    if (flag)
    {
      if (v[i] == 1)
      {
        count++;
      }
    }
    if (cnt == n)
    {
      flag = !flag;
      cnt = 0;
    }
  }

  if (count % 2 == 0)
  {
    return 1;
  }
  return 0;
}

pair<vector<int>, vector<int>> calcCodeWord(string dataword, bool evenP)
{
  reverse(dataword.begin(), dataword.end());
  int n = dataword.length();
  int r = 0;
  while (pow(2, r) <= n + r + 1)
  {
    r++;
  }

  vector<int> v(r + n + 1, 0);

  int power = 0;
  int ind = 0;
  for (int i = 1; i <= r + n; i++)
  {
    if (i == pow(2, power))
    {
      power++;
    }
    else
    {
      v[i] = dataword[ind] - 48;
      ind++;
    }
  }
  vector<int> parityBits(r, 0);
  power = 0;

  if (evenP)
  {
    for (int i = 0; i < r; i++)
    {
      parityBits[i] = evenParity(pow(2, power), v);
      power++;
    }
  }
  else
  {
    power = 0;
    for (int i = 0; i < r; i++)
    {
      parityBits[i] = oddParity(pow(2, power), v);
      power++;
    }
  }
  power = 0;
  for (int i = 0; i < parityBits.size(); i++)
  {
    v[pow(2, power)] = parityBits[i];
    power++;
  }
  return {v, parityBits};
}

int main()
{
  string word, result = "";

  cout << "Enter the word: ";
  cin >> word;
  vector<vector<int>> sentence;
  vector<vector<int>> allParityBits;
  for (int i = 0; i < word.size(); i++)
  {
    cout << "Ascii value of " << word[i] << " " << int(word[i]) << " ";
    cout << "Binary value" << " " << decToBinary(word[i]) << " ";
    cout << endl;
  }

  bool parity, error;
  cout << "Even parity or Odd parity (1/0): ";
  cin >> parity;
  for (int i = 0; i < word.size(); i++)
  {
    auto [code, parityBits] = calcCodeWord(decToBinary(word[i]), parity);
    reverse(code.begin(), code.end());
    sentence.push_back(code);
    allParityBits.push_back(parityBits);
  }

  cout << "The code word is: ";
  for (int i = 0; i < sentence.size(); i++)
  {
    for (int j = 0; j < sentence[i].size() - 1; j++)
    {
      cout << sentence[i][j];
    }
    cout << " ";
    reverse(sentence[i].begin(), sentence[i].end());
  }
  cout << endl;

  cout << "The original parity bits for each character are: " << endl;
  for (int i = 0; i < allParityBits.size(); i++)
  {
    cout << "Character '" << word[i] << "': ";
    for (int bit : allParityBits[i])
    {
      cout << bit;
    }
    cout << endl;
  }

  cout << "Do you want to generate error? (1/0): ";
  cin >> error;

  int letter = rand() % sentence.size();
  int position = rand() % sentence[letter].size() + 1;
  // receiver side code
  if (error)
  {
    sentence[letter][position] = !sentence[letter][position];
    cout << "Error generated in letter " << letter + 1 << ", position " << position << endl;
  }

  cout << "\nReceiver side:\n";
  for (int i = 0; i < sentence.size(); i++)
  {
    int r = log2(sentence[i].size());
    vector<int> recalculatedParityBits(r+1);

    cout << "Recalculated parity bits for character " << word[i] << ": ";
    for (int k = 0; k <=r; k++)
    {
      int parityBit;
      if (parity)
      {
        parityBit = evenParity(pow(2, k), sentence[i]);
      }
      else
      {
        parityBit = oddParity(pow(2, k), sentence[i]);
      }
      recalculatedParityBits[k] = parityBit;
      cout << parityBit;
    }
    cout << endl;

    bool errorDetected = false;
    for (int k = 0; k < r; k++)
    {
      if (recalculatedParityBits[k] != 0)
      {
        errorDetected = true;
        break;
      }
    }

    if (errorDetected)
    {
      cout << "Error detected in letter " << i + 1 << endl;
      error = true;
    }
    else if (!error)
    {
      string bin = "";
      int power = 0;

      for (int k = 1; k < sentence[i].size(); k++)
      {
        if (k != pow(2, power))
        {
          bin += to_string(sentence[i][k]);
        }
        else
        {
          power++;
        }
      }
      reverse(bin.begin(), bin.end());
      int g = stoi(bin, 0, 2);
      result += char(g);
    }
  }

  if (!error)
  {
    cout << "The decoded message is: ";
    cout << endl;
    cout << result;
  }
  else
  {
    cout << "Error detected. Unable to decode the message correctly." << endl;
  }

  return 0;
}