#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>   // for strlen
using namespace std;

struct Student {
    int id;
    char name[20];
    double score;
};

// 文本模式写入
void writeText(const string& filename) {
    ofstream out(filename);
    if (!out) { cerr << "Error opening " << filename << endl; return; }
    out << "ID\tName\tScore\n";
    out << 101 << "\tAlice\t" << 92.5 << "\n";
    out << 102 << "\tBob\t" << 88.0 << "\n";
    out.close();
    cout << "Text file written: " << filename << endl;
}

// 文本模式读取
void readText(const string& filename) {
    ifstream in(filename);
    if (!in) { cerr << "Error opening " << filename << endl; return; }
    string line;
    while (getline(in, line)) {
        cout << line << '\n';
    }
    in.close();
}

// 二进制写入固定长度记录
void writeBinary(const string& filename) {
    vector<Student> students = {
        {1, "Alice", 92.5},
        {2, "Bob", 88.0},
        {3, "Charlie", 76.3}
    };
    ofstream out(filename, ios::binary);
    if (!out) { cerr << "Error opening binary file" << endl; return; }
    for (const auto& s : students) {
        out.write(reinterpret_cast<const char*>(&s), sizeof(s));
    }
    out.close();
    cout << "Binary file written: " << filename << endl;
}

// 二进制随机访问读取记录
void readRecord(const string& filename, int recordNumber) {
    ifstream in(filename, ios::binary);
    if (!in) { cerr << "Error opening binary file" << endl; return; }
    Student s;
    streampos pos = (recordNumber - 1) * sizeof(Student);
    in.seekg(pos);
    in.read(reinterpret_cast<char*>(&s), sizeof(s));
    if (in) {
        cout << "Record " << recordNumber << ": " 
             << s.id << ", " << s.name << ", " << s.score << endl;
    } else {
        cerr << "Read error or record not found.\n";
    }
    in.close();
}

// 字符串流：格式化输出
string formatScore(const string& name, double score) {
    ostringstream oss;
    oss << "Student " << name << " scored " << score;
    return oss.str();
}

// 字符串流：解析CSV行
void parseCSV(const string& line) {
    istringstream iss(line);
    string token;
    while (getline(iss, token, ',')) {
        cout << "[" << token << "] ";
    }
    cout << endl;
}

int main() {
    // 文本
    writeText("test.txt");
    cout << "--- reading text ---\n";
    readText("test.txt");
    
    // 二进制
    writeBinary("students.bin");
    cout << "--- reading binary record 2 ---\n";
    readRecord("students.bin", 2);
    
    // 字符串流
    cout << formatScore("Dave", 95.0) << endl;
    parseCSV("102,Bob,88.0");
    return 0;
}