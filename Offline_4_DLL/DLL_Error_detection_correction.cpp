#include <iostream>
#include <string>
#include <bitset>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <Windows.h>
#include <random>


using namespace std;

class Ham{
    public:
    int* hammingCode;
    int size;
    Ham(int size,int* hammingCode){
        this->hammingCode=hammingCode;
        this->size=size;
    }   
};

int binaryToDecimal(const string& binaryStr) {
    bitset<64> bits(binaryStr); 
    return static_cast<int>(bits.to_ulong());
}

Ham hamming_code_gen(int m,int* bitArray){
    int parityBitcount=0;
    while(true){
        if((pow(2,parityBitcount)-(parityBitcount+1))>=m)
            break;
        parityBitcount++;
    }
    int hamSize=parityBitcount+m;
    int* ham=new int[hamSize]();
    int pow2=1;
    int cnt=0;
    for(int i=0;i<hamSize;i++){
        if(i+1!=pow2){
            ham[i]=bitArray[cnt];
            cnt++;
        }
        else{
            pow2=pow2*2;
        }
    }
    int c=1;
    for(int i=0;i<parityBitcount;i++){
        int c2=c;
        bool par=false;
        for(int j=0;j<=hamSize;j++){
            if(c2==0){
                par=(!par);
                c2=c;
                if(j!=0)
                    if(par)ham[(1 << i)-1]=ham[(1 << i)-1]^ham[j-1];    
                c2--;
            }else{
                if(j!=0)
                    if(par)
                        ham[(1 << i)-1]=ham[(1 << i)-1]^ham[j-1];         
                c2--;
            }
        }
        c=c*2;
    }
    Ham h(hamSize,ham);
    return h;
}




int* correctCode(int* ham,int m){
    int parityBitcount=0;
    while(true){
        if((pow(2,parityBitcount)-(parityBitcount+1))>=m)
            break;
        parityBitcount++;
    }
    int hamSize=parityBitcount+m;
    int c=1;
    int* hamCorrected=new int[hamSize];
    for(int i=0;i<hamSize;i++)
        hamCorrected[i]=ham[i];
    int* parArray=new int[parityBitcount];

    for(int i=0;i<parityBitcount;i++){
        int c2=c;
        bool par=false;
        for(int j=0;j<=hamSize;j++){
            if(c2==0){
                par=(!par);
                c2=c;
                if(j!=0)
                    if(par && ((1 << i)-1)!=(j-1))
                        ham[(1 << i)-1]=ham[(1 << i)-1]^ham[j-1];
                c2--;
            }else{
                if(j!=0)
                    if(par && ((1 << i)-1)!=(j-1))
                        ham[(1 << i)-1]=ham[(1 << i)-1]^ham[j-1];  
                c2--;
            }
        }
        parArray[i]=ham[(1 << i)-1];
        c=c*2;
        
    }
    int* parArray2=new int[parityBitcount];
    for(int i=0;i<parityBitcount;i++)
        parArray2[parityBitcount-i-1]=parArray[i];
    string result;
    for (int i=0;i<parityBitcount;i++) {
        result += std::to_string(parArray2[i]);
    }
    int a=binaryToDecimal(result);
    if(a==0);
        //cout<<"No error detected"<<endl;
    else{
        cout<<"Error in bit: "<<a<<endl;
        hamCorrected[a-1]=!hamCorrected[a-1];
    }
    

    int* newCode=new int[m*8];
    int cnt=0;
    int n=0;
    for(int i=0;i<hamSize;i++){
        if((i+1)==pow(2,n)){
            n++;
        }else{
            newCode[cnt]=hamCorrected[i];
            cnt++;
        }
    }
    return newCode;
}

int* collumWiseSerial(int** hamRow,int r,int c){
    int* serialData=new int[r*c];
    int cnt=0;
    for(int i=0;i<c;i++)
        for(int j=0;j<r;j++){
            serialData[cnt]=hamRow[j][i];
            cnt++;
        }    

    return serialData;  
}

int* flush(int* arr,int size){
    for(int i=0;i<size;i++)
        arr[i]=0;
    return arr;
}

int* crc(int* key,int key_length,int* dataOld,int data_length){
    int* data=new int[data_length+key_length-1];
    for(int i=0;i<data_length+key_length-1;i++)
        if(i<data_length)
            data[i]=dataOld[i];
        else
            data[i]=0; 
   
    for (int i = 0; i < data_length; i++) {
        if (data[i] == 1) {
            for (int j = 0; j < key_length; j++) {
                data[i + j] ^= key[j];
            }
        }
    }
    
    for(int i=0;i<data_length;i++)
        data[i]=dataOld[i];

    return data;
}

bool crcTest(int* key,int key_length,int* data,int data_length) {

    for (int i = 0; i < data_length; i++) {
        if (data[i] == 1) {
            for (int j = 0; j < key_length; j++) {
                data[i + j] ^= key[j];
            }
        }
    }
    
    for (int i = data_length; i < data_length+key_length-1; i++) {
        if (data[i] != 0) {
            return false; 
        }
    }
    
    return true;
}

int main() {
    string sentence ;
    int m;
    int p;
    string gen;
    cout << "Enter a line of text (space allowed): ";
    getline(cin, sentence);
    cout << "Character per row: ";
    cin>>m;
    cout<<"Probability of a bit flipping from 0 to 100:"<<endl;
    cin>>p;
    cout<<"generator polynomial: ";
    cin>>gen;
    int characterCount = sentence.length();
    while(characterCount%m!=0){
        sentence=sentence+"~";
        characterCount = sentence.length();
    }
    //cout<<characterCount/m<<endl;
    cout<<endl;
    cout<<"data string after padding: "<<sentence;

    vector<string> splitted; 
    for (int i = 0; i < sentence.length(); i += m) {
        splitted.push_back(sentence.substr(i, m));
    }

    int** row=new int*[characterCount/m];
    for(int k=0;k<characterCount/m;k++){
        int* bitArray=new int[m*8];
        int j=0;
        for (char c : splitted.at(k)) {
            int byteValue = static_cast<int>(c);
            bitset<8> bits(byteValue);
            for (int i = 7; i >-1; i--) {
                bitArray[j] = bits[i];
                j++;
        }
        //cout <<bitset<8>(byteValue) << " ";
        }
    cout<<endl;
    row[k]=bitArray;
    }

    cout<<"data block (ascii code of m characters per row): "<<endl;
    for(int i=0;i<characterCount/m;i++){
        for(int j=0;j<m*8;j++)
            cout<<row[i][j];
        cout<<endl;
    }

    int** hamRow=new int*[characterCount/m];
    cout<<endl;
    cout<<"data block after adding check bits: "<<endl;
    int hammmingSize=0;
    for(int i=0;i<characterCount/m;i++){
        Ham h=hamming_code_gen(m*8,row[i]);
        hamRow[i]=h.hammingCode;
        int cnt1=0;
        for(int j=0;j<h.size;j++){
            if((j+1)==pow(2,cnt1)){
                cnt1++;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
            }else
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            cout<<hamRow[i][j];
        }  
        cout<<endl;
        hammmingSize=h.size;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

    int* serialData=collumWiseSerial(hamRow,characterCount/m,hammmingSize);
    cout<<endl;
    cout<<"data bits after column-wise serialization: "<<endl;
    for(int i=0;i<hammmingSize*(characterCount/m);i++)
        cout<<serialData[i];
    cout<<endl;

    int* genArr=new int[gen.length()];
    int cc=0;
    for (char c : gen) {
        if(c=='1')
        genArr[cc]=1;
        else if(c=='0')
        genArr[cc]=0;
        else  cout<<"invalid input of polynomial"<<endl;
        cc++;
    }
    
    int* crcAdded=crc(genArr,gen.length(),serialData,hammmingSize*(characterCount/m));
    cout<<endl;
    cout<<"data bits after appending CRC checksum (sent frame): "<<endl;
    for(int i=0;i<hammmingSize*(characterCount/m)+gen.length()-1;i++){
        if(i>=hammmingSize*(characterCount/m))
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);  
        cout<<crcAdded[i];
    }   
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout<<endl;

    vector<int> errorPos;
    vector<int> errorPos2;
    for(int i=0;i<hammmingSize*(characterCount/m)+gen.length()-1;i++){
    const unsigned int seed = i;
    mt19937 rng(seed);
    uniform_real_distribution<double> dist(0, 100);
    int random_number = dist(rng);
    if(random_number<p){
        crcAdded[i]=!crcAdded[i];
        errorPos.push_back(i);
        errorPos2.push_back(i);
    }
        
    }
    cout<<endl;
    cout<<"received frame: "<<endl;
    for(int i=0;i<hammmingSize*(characterCount/m)+gen.length()-1;i++){
        if(errorPos.size()>0)
        if(errorPos.at(0)==i){
            errorPos.erase(errorPos.begin());
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
        }
        cout<<crcAdded[i];
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    }
        

    int* crcAdded2=new int[hammmingSize*(characterCount/m)+gen.length()-1];
    for(int i=0;i<hammmingSize*(characterCount/m)+gen.length()-1;i++)
        crcAdded2[i]=crcAdded[i];
    cout<<endl;
    cout<<endl;
    cout<<"result of CRC checksum matching: ";
    if(crcTest(genArr,gen.length(),crcAdded2,hammmingSize*(characterCount/m)))
        cout<<"no error"<<endl;
    else
        cout<<"error detected"<<endl;
    cout<<endl;

    int cnt2=0;
    vector<pair<int,int>> pairV;
    for(int j=0;j<hammmingSize;j++)
        for(int i=0;i<characterCount/m;i++){
        if(errorPos2.size()>0)
        if(errorPos2.at(0)==cnt2){
            errorPos2.erase(errorPos2.begin());
            pairV.push_back(make_pair(i,j));
        }
            hamRow[i][j]=crcAdded[cnt2];
            cnt2++;

        }
        
    cout<<"data block after removing CRC checksum bits: "<<endl;
    for(int i=0;i<characterCount/m;i++){
        for(int j=0;j<hammmingSize;j++){
        if(pairV.size()>0)
            for (const auto& pair : pairV) {
                    if(pair.first==i&&pair.second==j)
                         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            }
            cout<<hamRow[i][j];
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        }
            
        cout<<endl;
    }
    cout<<endl;

    cout<<"data block after removing check bits: "<<endl;
    for(int i=0;i<characterCount/m;i++){
        int* crHam=correctCode(hamRow[i],m*8);
        hamRow[i]=crHam;
        for(int j=0;j<m*8;j++)
            cout<<crHam[j];
        cout<<endl;
    }


    cout<<endl;
    cout<<"output frame: ";
    
    for(int i=0;i<characterCount/m;i++){
        
        string s1="";
        for(int j=0;j<m*8;j++){
            if(hamRow[i][j]==1)
                s1=s1+"1";
            else
                s1=s1+"0";
            if((j+1)%8==0){
                bitset<8> binaryArray(s1);
                char asciiChar = static_cast<char>(binaryArray.to_ulong());
                cout<<asciiChar;
                s1="";
            }
        }
    }
     
    return 0;
}


