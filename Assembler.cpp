#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include <fstream>
using namespace std;

string function(int n) {
    string binary;
    while (n > 0) {
        binary = char('0' + (n % 2)) + binary;
        n /= 2;
    }
    return binary;
}

string two_complement(int a) {
    if (a >= 0) {
        string c = "";
        c+= "0";
        c+= function(a);
        return c;
    } else {
        int positive_value = abs(a);
        string inverted_bits;
        for (char bit : (function(positive_value))) {
            inverted_bits += (bit == '0') ? '1' : '0';
        }
        int carry = 1;
        for (int i = inverted_bits.size() - 1; i >= 0; --i) {
            int sum = (inverted_bits[i] - '0') + carry;
            inverted_bits[i] = char('0' + (sum % 2));
            carry = sum / 2;
        }
        string e = "1";
        e+= inverted_bits;
        return e ;
    }
}
string sign_extension (int number , int target )
{
    string c = "";
    string d = two_complement(target);
    if (target<0&& number>d.length())
    {
        for (int i =1 ; i<=number-d.length() ; i++){
        c+= "1";
        }
        c+= two_complement(target);
    }
    else if (target>=0 && number > d.length())
    {
        for (int i =1 ; i<=number-d.length() ; i++){
        c+= "0";
        }
        c+= d;
    }
    return c ;
}
vector<string> splitString(string s, char t) {
    vector<string> arr;
    stringstream ss(s);
    string token;
    while (getline(ss, token, t)) {
        arr.push_back(token);
    }
    return arr;
}
vector<string> splitString_m(const string& input, const string& delimiters) {
    vector<string> tokens;
    stringstream ss(input);
    string token;
    while (getline(ss, token)) {
        size_t start = 0, end = 0;
        while ((start = token.find_first_not_of(delimiters, end)) != string::npos) {
            end = token.find_first_of(delimiters, start);
            tokens.push_back(token.substr(start, end - start));
        }
    }
    return tokens;
}

int b_to_d(int n){
    int c = 1 ; 
    int d = 1 ;
    int sum = 0;
    while (c>=1)
    {
        int e = n%10;
        sum = sum + e*d;
        d*=2;
        c = n/10;    
        n = c ;
    }
    return sum;
}


int d_to_b(int n){
    int s = 0 ;
    int c = 1 ;
    int y = 1;
    while(c>=1){ 
        int d = n%2; 
        s = s+ d*y ;
        y = y*10;
        c = n/2 ; 
        n = c ;
    }
    return s;
}


int find(string arr[], int size, string target) {
    for (int i = 0; i < size; ++i) {
        if (arr[i] == target) {
            return i;
        }
    }
  return 0;
}

string b_for_r(const vector<string>& arr) {
    string rs1 = arr.at(1);
    string rs2 = arr.at(2);
    string rs3 = arr.at(3);
    string u = "";
    u.append("0110011");
    u.append(rs1);
    string arr_f[9] = {"add","sub","sll","slt","sltu","xor","srl","or","and"};
    for(int i = 0; i < 9; i++) {
    if(arr.at(0)=="add" || arr.at(0)=="sub") {
        u.append("000");
        break;
    } else {
        u.append(to_string(d_to_b(find(arr_f,9,arr.at(0))-1)));
        break;
    }
}

    u.append(rs2);
    u.append(rs3);
    if(arr.at(0) == "sub") {
        u.append("0100000 ");
    } else {
        u.append("0000000 ");
    }
    return u;
}


string b_for_i(string s){
  if(s[0]=='l'){
    vector<string> arr = splitString_m(s, ", ( )");
  }
  else{
    vector<string> arr= splitString(s, ',');
  }
  string rs1 = arr.at(1);
  string rs2 = arr.at(2);
  string rs3 = arr.at(3);
  string u = "";
  if(arr.at(0) == "lw"){
    u.append("0000011");
  }
  else if(arr.at(0) == "addi" || arr.at(0) == "sltiu"){
    u.append("0010011");
  }
  else{
    u.append("1100111");
  }
  u.append(rs1);
  string s = "";
  if(arr.at(0)=="addi" || arr.at(0)=="jalr"){
    s = "000";
  }
  else if(arr.at(0)=="sltiu"){
    s = "011";
  }
  else{
    s = "010";
  }
  u.append(s);
  if(arr.at(0)=="lw"){
    u.append(rs3);
    u.append(rs2);
  }
  else{
    u.append(rs2);
    u.append(rs3);
  }
  return u;
}


string S_type(int rs1, int rs2, int immediate) {
    bitset<32> instruction;
    int x = d_to_b(immediate);
    string y = to_string(x);
    string y1 =y.substr(0,4);
    string y2 =y.substr(4,11);
    int immediate1 = b_to_d(stoi(y1));
    int immediate2 = b_to_d(stoi(y2));
    cout << immediate1 << immediate2<< endl;
  
    int opcode = 35;
    int func3 = 2;

    instruction |= (bitset<32>(immediate2) << 25);
    instruction |= (bitset<32>(rs1) << 20);
    instruction |= (bitset<32>(rs2) << 17);
    instruction |= (bitset<32>(func3) << 12);
    instruction |= (bitset<32>(immediate1) << 7);
    instruction |= bitset<32>(opcode);

    return instruction.to_string();
}
string b_for_b(const vector<string>& arr){
  string rs1 = arr.at(1);
  string rs2 = arr.at(2);
  string rs3 = arr.at(3);
  string u = "";
  u.append("1100011");
  u.append(rs3.substr(0,5));
  string s = "";
  if(arr.at(0)=="beq")
  {
    s = "000";
  }
  else if(arr.at(0)=="bne")
  {
    s = "001";
  }
  else if(arr.at(0)=="blt")
  {
    s = "100";
  }
  else if(arr.at(0)=="bge")
  {
    s = "101";
  }
  else if (arr.at(0)=="bltu")
  {
    s = "110";
  }
  else
  {
    s = "111";
  }
  u.append(s);
  u.append(rs1);
  u.append(rs2);
  u.append(rs3.substr(5,7));
  return u;
}


string b_for_u(const vector<string>& arr){
  string u = "";
  if(arr.at(0) == "lui"){
    u.append("0110111");
  }
  else{
    u.append("0010111");
  }
  u.append(arr.at(1));
  u.append(arr.at(2));
  return u;
}

string b_for_j(const vector<string>& arr) {
      string u = "";
      u.append("1101111");
      string rd = arr.at(1);
      string imm = arr.at(2);
      int immediate;
      if (imm.substr(0, 2) == "0x") {
          immediate = stoi(imm, nullptr, 16);
      }
      else {
          immediate = stoi(imm);
      }
      string binary_imm;
      if (immediate >= 0) {
          binary_imm = bitset<20>(immediate).to_string();
      }
      else {
          binary_imm = bitset<20>(immediate & 0xFFFFF).to_string();
      }
      u.append(binary_imm);

      u.append(rd);

      return u;
  }

int main(){
  string reg[32] = {"x0","x1","x2","x3","x4","x5","x6","x7","x8","x9","x10","x11","x12","x13","x14","x15","x16","x17","x18","x19","x20","x21","x22","x23","x24","x25","x26","x27","x28","x29","x30","x31"};
  string arr_v[32]={};
  for(int i=0; i<32; i++){
    arr_v[i]=to_string(d_to_b(i));
    if (arr_v[i].length() < 5) {
        arr_v[i]= string(5 - arr_v[i].length(), '0') + arr_v[i];
    }
  }
  int n;// no. of instructions
  cin>>n;
  vector<string> assm(n);
  for(int i=0;i<n;i++){
    cin>>assm[i];
  }
  for(int i=0; i<n; i++){
    string token;
    vector<string> arr_t;
    for (char c : assm[i]) {
        if (c == ',') {
            arr_t.push_back(token);
            token = "";
        } else {
            token += c;
        }
    }
    arr_t.push_back(token);
  }


  // output use below line
  int rs1 = 1, rs2 =2, immediate = 32;
  string output = S_type(rs1 , rs2, immediate);
  cout << output << endl;

 
}
