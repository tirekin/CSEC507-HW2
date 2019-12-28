#include <iostream>
#include <bitset>
#include <fstream>
#include <vector>

using namespace std;

void printArray(int *arr, int n) {
    for(int i=0; i<n; i++) {
        switch(arr[i]) {
            case 10:
                cout << "A";
                break;
            case 11:
                cout << "B";
                break;
            case 12:
                cout << "C";
                break;
            case 13:
                cout << "D";
                break;
            case 14:
                cout << "E";
                break;
            case 15:
                cout << "F";
                break;
            default:
                cout << arr[i];
        }
    }
    cout << endl;
}

char convertIntToHex(int i) {
    switch(i) {
        case 10:
            return 'A';
        case 11:
            return 'B';
        case 12:
            return 'C';
        case 13:
            return 'D';
        case 14:
            return 'E';
        case 15:
            return 'F';
        default:
            return '?';
    }
}

int convertStrToHex(char c) {
    switch(c) {
        case 'A':
            return 10;
        case 'B':
            return 11;
        case 'C':
            return 12;
        case 'D':
            return 13;
        case 'E':
            return 14;
        case 'F':
            return 15;
        default:
            return c - '0';
    }
}

void print_2d_vector(vector<vector<bool>> matrix) {
    for(int i=0; i<matrix.size(); i++) {
        for(int j=0; j<matrix[i].size(); j++) {
            cout << " " << matrix[i][j];
        }
        cout << endl;
    }
}

void print_vector(vector<bool> vec) {
    for(int i=0; i<vec.size(); i++) {
        cout << " " << vec[i];
    }

    cout << endl;
}

int bitDifference(int n1, int n2) {
    int difference = 0;
    for(int i=0; i<32; i++) {
        if(((n1 >> i) & 1) != ((n2 >> i) & 1)) {
            difference++;
        }
    }
    return difference;
}

void diffFactorCheck(int x, int y, int output_difference, int DDTMatrix[16][16], int keys[16], int values[16]) {
    bool res[16];
    for(int i=0; i<16; i++) {
        if(values[x ^ i] ^ values[y ^ i] == output_difference) {
            
        }
    }
}

void printDDT(int keys[16], int values[16]) {
    int diff_uniformity = INT32_MIN;
    int DDTMatrix[16][16]; // DDT Table
    int outputs[16][16]; // rows: input difference, cols: output difference values (will be used for undisturbed bit calculations)
    int no_of_outputs[16]; // Number of outputs the input difference currently has (just need it for array manipulation)
    int output_differences[16][16];

    // Initialize DDT Matrix
    for(int i=0; i<16; i++) {
        no_of_outputs[i] = 0;
        for(int j=0; j<16; j++) {
            DDTMatrix[i][j]=0;
        }
    }

    // Compute DDT Matrix
    for(int i=0; i<16; i++) {
        for(int j=0; j<16; j++) {
            int input_difference = i ^ j;
            int output_difference = values[i] ^ values[j];
            DDTMatrix[input_difference][output_difference]++;
            outputs[input_difference][no_of_outputs[input_difference]] = output_difference;
            no_of_outputs[input_difference]++;
            output_differences[i][j] = output_difference;
            //diffFactorCheck(i, j, output_difference, DDTMatrix, keys, values);
        }
    }
    
    cout << "*** DIFFERENTIAL FACTOR ANALYSIS ***" << endl;
    for(int out_diff=0; out_diff<16; out_diff++) {
        vector<bool> satisfies(16, true);
        for(int x=0; x<16; x++) {
            for(int y=0; y<16; y++) {
                if(output_differences[x][y] != out_diff) continue;
                for(int lambda=0; lambda<16; lambda++) {
                    int res = (values[x ^ lambda] ^ values[y ^ lambda]);
                    if(res != out_diff) {
                        satisfies[lambda] = false;
                    }
                }
            }
        }

        if(out_diff != 0) {
            for(int i=0; i<16; i++) {
                if(i!=0 && satisfies[i] == true) {
                    if(out_diff<10) cout << "Output difference:" << out_diff << " ";
                    else cout << "Output difference:" << convertIntToHex(out_diff) << " ";
                    if(i<10) cout << "Satisfies for lambda " << i << endl;
                    else cout << "Satisfies for lambda " << convertIntToHex(i) << endl; 
                }
            }
        }
    }
    cout << "******" << endl;

    cout << "DDT TABLE:" << endl;
    // Calculate differential uniformity.
    for(int i=0; i<16; i++) {
        for(int j=0; j<16; j++) {
            if(j != 0)
                cout << " " << DDTMatrix[i][j];
            else
                cout << DDTMatrix[i][j];
            if(i==0 && j==0) continue;
            if(DDTMatrix[i][j] > diff_uniformity)
                diff_uniformity = DDTMatrix[i][j];
        }
        cout << endl;
    }
    cout << "Differential uniformity of DDT: " << diff_uniformity << endl;
    cout << "*** UNDISTURBED BIT ANALYSIS ***" << endl;
    // Calculate undisturbed bits.
    for(int inp_diff=0; inp_diff<16; inp_diff++) {
        int bits[4];
        bool changed[4];
        for(int k=0; k<4; k++) {
            bits[k] = bitset<4>(outputs[inp_diff][0])[k];
            changed[k] = false;
        }
        //cout << "Output differences for input diff " << inp_diff << endl;
        for(int j=0; j<16; j++) {
            bitset<4> current_bits = bitset<4>(outputs[inp_diff][j]);
            //cout << current_bits << endl;
            for(int m=0; m<4; m++) {
                if(current_bits[m] != bits[m]) {
                    changed[m] = true;
                }
            }
        }
        for(int k=0; k<4; k++) {
            if(!changed[k] && inp_diff != 0) {
                cout << "Undisturbed bit for input difference " << inp_diff << ": " << k << ", value:" << bits[k] << endl;
            }
        }
        //cout << endl << "------------" << endl;
    }
    cout << "******" << endl;
}

int mask(int a, int b) {
    bitset<4> a_bits = bitset<4>(a);
    bitset<4> b_bits = bitset<4>(b);
    int result = 0;
    for(int i=0; i<4; i++) {
        result ^= a_bits[i] & b_bits[i];
    }
    //cout << "mask result: " << result << endl;
    return result;
}

int computeLATValue(int i, int j, int keys[16], int values[16]) {
    int result = 0;
    for(int x=0; x<16; x++) {
        result += mask(i,x) ^ mask(j, values[x]);
    }
    return result - 8;
}

void printLAT(int keys[16], int values[16]) {
    int LATMatrix[16][16];
    int maxEntry = INT32_MIN;
    for(int i=0; i<16; i++) {
        for(int j=0; j<16; j++) {
            LATMatrix[i][j] = computeLATValue(i, j, keys, values);
            cout << " " << LATMatrix[i][j];
            if(i==0 && j==0) continue;
            if(abs(LATMatrix[i][j]) > maxEntry) {
                maxEntry = abs(LATMatrix[i][j]);
            }
        }
        cout << endl;
    }
    cout << "Differential uniformity of LAT: " << maxEntry << endl;
}

void parse_file(char *file_name) {
    ifstream file(file_name);
    int n = 0;
    if(file.is_open()) {
        while(true) {
            int keys[16];
            int values[16];
            string line;
            if(!getline(file,line)) break;
            string keys_str = line;
            if(!getline(file,line)) break;
            string values_str = line;
            for(int i=0; i<16; i++) {
                keys[i] = convertStrToHex(keys_str[i]);
                values[i] = convertStrToHex(values_str[i]);
            }
            int normalized_keys[16];
            int normalized_values[16];
            for(int i=0; i<16; i++) {
                normalized_keys[keys[i]] = keys[i];
                normalized_values[keys[i]] = values[i];
            }
            cout << "****** SBOX NO: " << n << " ******" << endl;
            cout << "Keys: ";
            printArray(normalized_keys, 16);
            cout << "Values: ";
            printArray(normalized_values, 16);
            cout << "DDT:" << endl;
            printDDT(normalized_keys, normalized_values);
            cout << "LAT:" << endl;
            printLAT(normalized_keys, normalized_values);
            cout << "************" << endl;
            n++;
        }
    }
}

int main(int argc, char *argv[]) {

    if(argc != 2) {
        cout << "Usage: ./ddt <filename>" << endl;
        return -1;
    }
    parse_file(argv[1]);

    return 0;
}