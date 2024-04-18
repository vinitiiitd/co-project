#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <bitset>

using namespace std;
map < string, string > value;

string reg[32] =
  { "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10", "x11",
"x12", "x13", "x14", "x15",
  "x16", "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25", "x26",
	"x27", "x28", "x29", "x30", "x31"
};

int pc = 0;

bool
isPresent (string arr[], int size, string target)
{
  for (int i = 0; i < size; i++)
	{
	  if (arr[i] == target)
		{
		  return true;
		}
	}
  return false;
}

int
b_to_d (int n)
{
  int c = 1;
  int d = 1;
  int sum = 0;
  while (c >= 1)
	{
	  int e = n % 10;
	  sum = sum + e * d;
	  d *= 2;
	  c = n / 10;
	  n = c;
	}
  return sum;
}

int
d_to_b (int n)
{
  int s = 0;
  int c = 1;
  int y = 1;
  while (c >= 1)
	{
	  int d = n % 2;
	  s = s + d * y;
	  y = y * 10;
	  c = n / 2;
	  n = c;
	}
  return s;
}

string
function (int n)
{
  string binary;
  while (n > 0)
	{
	  binary = char ('0' + (n % 2)) + binary;
	  n /= 2;
	}
  return binary;
}

string
two_complement (int a)
{
  if (a >= 0)
	{
	  string c = "";
	  c += "0";
	  c += function (a);
	  return c;
	}
  else
	{
	  int positive_value = abs (a);
	  string inverted_bits;
	for (char bit:(function (positive_value)))
		{
		  inverted_bits += (bit == '0') ? '1' : '0';
		}
	  int carry = 1;
	  for (int i = inverted_bits.size () - 1; i >= 0; --i)
		{
		  int sum = (inverted_bits[i] - '0') + carry;
		  inverted_bits[i] = char ('0' + (sum % 2));
		  carry = sum / 2;
		}
	  string e = "1";
	  e += inverted_bits;
	  return e;
	}
}

string
sext (int number, int target_length)
{
  string d = two_complement (number);
  int current_length = d.length ();

  if (current_length < target_length)
	{
	  char sign_bit = (d[0] == '1') ? '1' : '0';
	  return string (target_length - current_length, sign_bit) + d;
	}
  else if (current_length > target_length)
	{
	  return d.substr (current_length - target_length);
	}
  else
	{
	  return d;
	}
}

string
binaryToHex (string binary)
{
  map < string, char >hexMap = {
	{"0000", '0'}, {"0001", '1'}, {"0010", '2'}, {"0011", '3'},
	{"0100", '4'}, {"0101", '5'}, {"0110", '6'}, {"0111", '7'},
	{"1000", '8'}, {"1001", '9'}, {"1010", 'A'}, {"1011", 'B'},
	{"1100", 'C'}, {"1101", 'D'}, {"1110", 'E'}, {"1111", 'F'}
  };

  int padding = (4 - binary.length () % 4) % 4;
  binary = string (padding, '0') + binary;

  string hex = "";
  for (int i = 0; i < binary.length (); i += 4)
	{
	  string group = binary.substr (i, 4);
	  hex += hexMap[group];
	}
  return hex;
}

int
bd (string binary)
{
  int decimal = 0;
  int sign = 1;

  if (binary[0] == '1')
	{
	  sign = -1;

	  for (int i = 0; i < binary.length (); ++i)
		{
		  binary[i] = (binary[i] == '0') ? '1' : '0';
		}

	  int carry = 1;
	  for (int i = binary.length () - 1; i >= 0; --i)
		{
		  int sum = (binary[i] - '0') + carry;
		  binary[i] = (sum % 2 == 0 ? '0' : '1');
		  carry = sum / 2;
		}
	}

  for (int i = binary.length () - 1; i >= 0; --i)
	{
	  if (binary[i] == '1')
		{
		  decimal += sign * pow (2, binary.length () - 1 - i);
		}
	}

  return decimal;
}

string
db (int decimalNum)
{
  return bitset < 32 > (decimalNum).to_string ();
}

map < string, string > data;	//key = mem, value = data
string mem[32];

void
initialise_mem ()
{
  for (int i = 0; i < 32; i++)
	{
	  if (i < 4)
		{
		  string val = to_string (d_to_b (4 * i));
		  string val1 = binaryToHex (val);
		  mem[i] = "0x0001000" + val1;
		}

	  else
		{
		  string val = to_string (d_to_b (4 * i));
		  string val1 = binaryToHex (val);
		  mem[i] = "0x000100" + val1;
		}
	  data[mem[i]] = "00000000000000000000000000000000";
	}
}





void
r_type (string fun)
{
  string rs2 = value[reg[b_to_d (stoi (fun.substr (7, 5)))]];
  string rs1 = value[reg[b_to_d (stoi (fun.substr (12, 5)))]];
  string rd = value[reg[b_to_d (stoi (fun.substr (20, 5)))]];
  string fun7 = (fun.substr (0, 7));
  string fun3 = (fun.substr (17, 3));
  int c = stoi (fun7 + fun3);
  switch (c)
	{
	case 0000000000:			// add
	  rd = sext ((b_to_d (stoi (rs1)) + b_to_d (stoi (rs2))), 32);
	  break;
	case 0100000000:
	  rd = sext ((b_to_d (stoi (rs1)) - b_to_d (stoi (rs2))), 32);
	case 0000000001:
	  rd = sext ((b_to_d (stoi (rs1)) << b_to_d (stoi (rs2))), 32);
	  break;
	case 0000000010:
	  if (b_to_d (stoi (rs1)) < b_to_d (stoi (rs2)))
		{
		  rd = "00000000000000000000000000000001";
		}
	  else
		{
		  rd = "00000000000000000000000000000000";
		}
	  break;
	case 0000000011:
	  if (stoull (rs1, nullptr, 2) < stoull (rs2, nullptr, 2))
		{
		  rd = "00000000000000000000000000000001";
		}
	  else
		{
		  rd = "00000000000000000000000000000000";
		}
	  break;
	case 0000000100:
	  rd = sext ((b_to_d (stoi (rs1)) ^ b_to_d (stoi (rs2))), 32);
	  break;
	case 0000000101:			// srl
	  rd = sext ((b_to_d (stoi (rs1)) >> b_to_d (stoi (rs2))), 32);
	  break;
	case 0000000110:			// or
	  rd = sext ((b_to_d (stoi (rs1)) | b_to_d (stoi (rs2))), 32);
	  break;
	case 0000000111:			// and
	  rd = sext ((b_to_d (stoi (rs1)) & b_to_d (stoi (rs2))), 32);
	  break;

	}
  pc += 4;
  for (int i = 0; i <= 31; i++)
	{
	  if (i == b_to_d (stoi (fun.substr (20, 5))))
		{
		  value[reg[i]] = rd;
		}
	}
}


void
i_type (string fun)
{
  int imm = bd (fun.substr (0, 12));
  int rs1 = stoi (fun.substr (12, 5));
  string fun3 = (fun.substr (17, 3));
  int rd = stoi (fun.substr (20, 5));
  string opcode = fun.substr (25, 7);

  string c = (fun3 + opcode);

  int PC = bd (sext (bd (value[reg[(6)]]) + imm, 32));
  //  cout<< imm <<endl;
//   cout<< rs1 << endl;
//   cout << fun3 <<endl;
  // cout<< rd <<endl;
//   cout<< opcode <<endl;
// cout<< b_to_d(stoi(value[reg[b_to_d(rs1)]])) <<endl;


  if (c == "0100000011" && (bd ((value[reg[b_to_d ((rs1))]])) + imm) < 32
	  && (bd ((value[reg[b_to_d ((rs1))]])) + imm) > 0)
	{							//lw  
	  value[reg[b_to_d (rd)]] =
		data[mem[bd (value[reg[b_to_d (rs1)]]) + imm]];
	  string result1 = value[reg[b_to_d (rd)]];

	}

  else if (c == "0000010011")
	{							//addi
	  value[reg[b_to_d (rd)]] =
		sext ((bd ((value[reg[b_to_d ((rs1))]])) + imm), 32);
	  string result2 = value[reg[b_to_d (rd)]];

	}

  else if (c == "0110010011")
	{							//sltiu
	  if (stoi (value[reg[b_to_d (rs1)]]) < imm)
		{
		  value[reg[b_to_d (rd)]] = sext (1, 32);
		}
	  string result3 = to_string (imm);

	}

  else if (c == "0001100111")
	{							//jalr
	  value[reg[b_to_d (rd)]] = sext ((PC + 4), 32);
	  string result4 = value[reg[b_to_d (rd)]];
	}
  pc += 4;

}

void
s_type (string fun)
{
  int imm = bd ((fun.substr (0, 7) + fun.substr (20, 5)));
  int rs2 = b_to_d (stoi (fun.substr (7, 5)));

  int rs1 = b_to_d (stoi (fun.substr (12, 5)));
  string fun3 = (fun.substr (17, 3));
  string opcode = fun.substr (25, 7);

  string c = (fun3 + opcode);
  if (c == "0100100011" && (bd ((value[reg[b_to_d (rs1)]])) + imm) < 32
	  && (bd ((value[reg[b_to_d (rs1)]])) + imm) > 0)
	{
	  data[mem[bd ((value[reg[b_to_d (rs1)]])) + imm]] =
		value[reg[b_to_d (rs2)]];
	}
  pc = pc + 4;
}

void
b_type (string fun)
{
  string imm = fun.substr (0, 1) + fun.substr (24, 1);
  imm += fun.substr (1, 5);
  imm += fun.substr (20, 4);
  cout<<imm<<endl;
  string rs2 = value[reg[b_to_d (stoi (fun.substr (7, 5)))]];
  //int a = b_to_d(stoi(fun.substr(7, 5)));
  //cout<<a<<endl;
  string rs1 = value[reg[b_to_d (stoi (fun.substr (12, 5)))]];
  //cout<<rs1<<endl;
  //cout<<rs2<<endl;
  string fun3 = fun.substr (17, 3);
  string opcode = (fun.substr (25, 7));
  string c = fun3 + opcode;
  if (c == "0001100011")
	{
	  if (bd (rs1) == bd (rs2))
		{
		  pc += bd ((imm + "0"));
		}
	  else
		{
		  pc += 4;
		}
	}
  else if (c == "0011100011")	// bne
	{
	  if (bd (rs1) != bd (rs2))
		{
		  pc += bd ( (imm + "0"));
		}
	  else
		{
		  pc += 4;
		}
	}
  else if (c == "1011100011")	//bge
	{
	  if (bd (rs1) >= bd (rs2))
		{
		  pc += bd ((imm + "0"));
		}
	  else
		{
		  pc = pc + 4;
		}
	}
  else if (c == "1001100011")	// blt
	{
	  if (bd (rs1) < bd (rs2))
		{
		  pc += bd ((imm + "0"));
		}
	  else
		{
		  pc += 4;
		}
	}
  else if (c == "1101100011")
	{
	  if (b_to_d (stoi (rs1)) < b_to_d (stoi (rs1)))
		{
		  pc += (bd ((imm + "0")));
		}
	  else
		{
		  pc += 4;
		}
	}
  else if (c == "1111100011")
	{
	  if (b_to_d (stoi (rs1)) >= b_to_d (stoi (rs2)))
		{
		  pc += bd ( (imm + "0"));
		}
	  else
		{
		  pc += 4;
		}
	}
  else
	{
	  pc += 4;
	}
  // for(int i=0;i<=31;i++){
  //     if(i==b_to_d(stoi(fun.substr(20,5)))){
  //         value[reg[i]]=rd;
  //     }
  // }
}

void
u_type (string fun)
{
  string imm = fun.substr (0, 20);
  string rd = value[reg[b_to_d (stoi (fun.substr (20, 5)))]];
  string opcode = fun.substr (25, 7);
  if (opcode == "0110111")
	{
	  int rc = pc + bd ((imm + "000000000000"));
	  rd = (two_complement (rc));
	  rd = sext (rc, 32);
	}
  else if (opcode == "0010111")
	{
	  rd = imm + "000000000000";
	}
  for (int i = 0; i <= 31; i++)
	{
	  if (i == b_to_d (stoi (fun.substr (20, 5))))
		{
		  value[reg[i]] = rd;
		}
	}
  pc += 4;
}

void
print_value ()
{
  cout << sext (pc, 32) << " ";
  cout << endl;
  for (int i = 0; i <= 32; i++)
	{
	  cout << value[reg[i]] << endl;
	}
  cout << endl;
}

void
print_mem ()
{
  cout << sext (pc, 32) << " ";
  for (int i = 0; i < 32; i++)
	{
	  cout << mem[i] << ":" << data[mem[i]] << endl;
	}
}

void
j_type (string fun)
{
  string imm =
	fun.substr (0, 1) + fun.substr (12, 8) + fun.substr (11,
														 1) + fun.substr (1,
																		  10);
  int imm1 = bd (imm + "0");

  string rd = value[reg[b_to_d (stoi (fun.substr (20, 5)))]];
  string opcode = fun.substr (25, 7);
  value[reg[b_to_d (stoi (rd))]] = sext (pc + 4, 32);
  pc = pc + imm1;
}

string
bonus (string fun, string reg[])
{
  string imm = (fun.substr (0, 7));

  int rs2 = b_to_d (stoi (fun.substr (7, 5)));
  int rs1 = b_to_d (stoi (fun.substr (12, 5)));
  string fun3 = fun.substr (17, 3);
  int rd = b_to_d (stoi (fun.substr (20, 5)));
  string opcode = (fun.substr (25, 7));
  string c = fun3 + opcode;


  if (c == "0000000001")
	{							//mul
	  value[reg[b_to_d (rd)]] =
		db (bd (value[reg[b_to_d (rs1)]]) * bd (value[reg[b_to_d (rs2)]]));
	  return value[reg[b_to_d (rd)]];
	}

  else if (c == "0001000001")
	{							//rst
	  for (int i = 0; i < 32; i++)
		{
		  if (i == 2)
			{
			  value[reg[i]] = "00000000000000000000000100000000";
			}
		  else
			{
			  value[reg[i]] = string (32, '0');
			}
		}
	}

  else if (c == "0001111111")
	{							// halt
	  exit (0);
	  return 0;
	}

  else if (c == "0010000001")
	{
	  reverse ((value[reg[b_to_d (rs1)]]).begin (),
			   (value[reg[b_to_d (rs1)]]).end ());
	  cout << 5 << endl;
	  cout << value[reg[b_to_d (rs1)]] << endl;
	  value[reg[b_to_d (rd)]] = value[reg[b_to_d (rs1)]];
	  return value[reg[b_to_d (rd)]];
	}

  return "";
}

int
main ()
{
  initialise_mem ();
  for (int i = 0; i < 32; i++)
	{
	  if (i == 2)
		{
		  value[reg[i]] = "00000000000000000000000100000000";
		}
	  else
		{
		  value[reg[i]] = string (32, '0');
		}
	}
  vector < string > assm;	//   string a;
string a;
  cin>>a;
  fstream myfile(a);
  string itr;
  while (getline (myfile, itr)) {
    assm.push_back(itr);
        
  }
  string arr1[1] = { "0110011"};
  string arr2[3] = { "0010011", "1100111" };
  string arr3[1] = { "0100011" };
  string arr4[1] = { "1100011" };
  string arr5[2] = { "0110111", "0010111" };
  string arr6[1] = { "1101111" };
  for (int i = 0; i < assm.size (); i++)
	{
	  if (isPresent (arr1, 1, assm[i].substr (25, 7)))
		{
		  r_type (assm[i]);
		  print_value ();
		}
	  else if (isPresent (arr2, 3, assm[i].substr (25, 7)))
		{
		    if(assm[i].substr(25,7)=="0000011"){
		  i_type (assm[i]);
		        
		    }
		    else{
		        i_type(assm[i]);
		        print_value();
		    }
		}
	  else if (isPresent (arr4, 1, assm[i].substr (25, 7)))
		{
		  b_type (assm[i]);
		  print_value ();
		}
	  else if (isPresent (arr4, 1, assm[i].substr (25, 7)))
		{
		  s_type (assm[i]);
		}
	  else if (isPresent (arr5, 2, assm[i].substr (25, 7)))
		{
		  u_type (assm[i]);
		  print_value ();
		}
	  else if (isPresent (arr6, 1, assm[i].substr (25, 7)))
		{
		  j_type (assm[i]);
		  print_value ();
		}
	}
	for(int i = 0 ; i < 32 ; i++){
    cout<<mem[i] << ":"<<data[mem[i]] <<endl;
  }
 



  //BONUS--BONUS
//string x = bonus("00000000100000100000000000000001",reg); //mul
//string x = bonus("00000000000000000000000001000001",reg); //rst
//string x = bonus("00000000000000000000000001111111",reg); //halt
// string x = bonus("00000000100000100001000000000001",reg); // rvrs
 //cout << x<<endl;
  return 0;
}




