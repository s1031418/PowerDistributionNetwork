//
//  ngspice.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 22/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "ngspice.hpp"

ngspice::ngspice()
{
    
}


void ngspice::init(string casename)
{
    setCaseName(casename);
    LoadFile();
    initvoltage();
}
void ngspice::LoadFile()
{
    string content ;
    string FilePath;
    FilePath.append(CaseName).append("_ngspice");
    fstream fin(FilePath , ios::in);
    if(!fin)
    {
        cerr << "Can't open ngspice file.";
        exit(1);
    }
    while (getline(fin,content)) {
        Data.push_back(content);
    }
    fin.close();
}
ngspice::~ngspice()
{
    
}
double ngspice::getIR_DropCons(string Block , string BlockPin )
{
    auto begin = ConstraintMaps.lower_bound(Block);
    auto end = ConstraintMaps.upper_bound(Block);
    while (begin != end)
    {
        if( begin->second.NAME == BlockPin )
        {
            return stod(begin->second.CONSTRAINT);
        }
        begin++;
    }
    return -1 ;
}
Point<int> ngspice::translateToPoint(string key)
{
    size_t first_pos = key.find("_");
    size_t last_pos = key.rfind("_");
    int x = stoi(key.substr(first_pos + 1 , last_pos - first_pos -1 ));
    int y = stoi(key.substr(last_pos + 1 ));
    return Point<int>(x,y);
}
void ngspice::setCaseName(string name)
{
    CaseName = name ; 
}
void ngspice::printStats(multimap<string, string> & DetinationMap)
{
    for( auto it = DetinationMap.begin(), end = DetinationMap.end(); it != end;it = DetinationMap.upper_bound(it->first))
    {
        auto beginning = DetinationMap.lower_bound(it->first);
        auto ending = DetinationMap.upper_bound(it->first);
        while ( beginning != ending )
        {
            double StartVoltage = voltages[beginning->first];
            double EndVoltage = voltages[beginning->second];
            Point<int> start = translateToPoint(beginning->first);
            Point<int> terminal = translateToPoint(beginning->second);
            pair<string, string> msg = helper.getBlockMsg(terminal);
            string PinName = helper.getPowerPinMsg(start);
            string BlockName = get<0>(msg);
            string BlockPinName = get<1>(msg);
            double Constraint = getIR_DropCons(BlockName, BlockPinName);
            double Drop_percent = ((StartVoltage - EndVoltage) / StartVoltage) * 100 ;
            cout << PinName << " "<< BlockName << " " << BlockPinName << " ";
            DropMap.insert(make_pair(make_pair(BlockName, BlockPinName), Drop_percent));
            if(EndVoltage < 0)
                cout << "Terminal Point Voltage is negative(NoPass)" << endl ;
            else if( Drop_percent > Constraint)
                cout << "Drop " << Drop_percent << "(%) (NoPass)"<< endl;
            else
                cout << "Drop " << Drop_percent << "(%) (Pass)" << endl;
            beginning++;
        }
    }
    cout << endl;
}
void ngspice::initvoltage()
{
    bool start = false ;
    for (int i = 0 ; i < Data.size(); i++)
    {
        if(Data[i].find("branch") != string::npos)
        {
            break;
        }
        if(start)
        {
            regex_t Regex ; // group 1、3
            if ( regcomp(&Regex, "(([[:alnum:]]|_|[.])+)[[:space:]]+(-?([[:alnum:]]|_|[.])+)", REG_EXTENDED) ) cerr << "Compile Error";
            vector<string> results = GetParsingString(Regex, Data[i], vector<int>{1,3});
            voltages.insert(make_pair(results[0], stod(results[1])));
            continue;
        }
        if( Data[i].find("Node") != string::npos )
        {
            start = true ;
            i += 1 ;
        }
    }
}
void ngspice::ConcatIR_Drop()
{
    
    string a ;
    string FilePath;
//    string input = CaseName.append("_ngspice");
    string input = FilePath.append(CaseName).append("_ngspice");
    fstream fin(input, ios::in);
    if(!fin) cerr << "Can't Read File" << endl;
    bool flag = 0 ;
    
    vector<string>string_vec;
    while (getline(fin,a))
    {
        if(a[0]=='N'&&a[1]=='o'&&a[2]=='d'&&a[3]=='e')
        {
            getline(fin,a);
            getline(fin,a);
            flag = 1 ;
        }
        if(flag == 1 )
        {
            if(a[0]=='v'&&a[1]=='_'&&a[2]=='v')
                break;
            string_vec.push_back(a);
        }
    }
    vector<vector<string > >tmp;
    tmp.resize(string_vec.size());
    for(int i = 0 ; i < string_vec.size();i++)
    {
        char *tp = new char [string_vec[i].length()+1];
        strcpy(tp,string_vec[i].c_str());
        char *p = strtok(tp,"_ ");
        while(p)
        {
            string t(p);
            tmp[i].push_back(t);
            p = strtok(NULL,"_ ");
        }
    }
    for(int i = 0;i<tmp.size();i++)
    {
        for(int j = 0 ; j<tmp.size();j++)
            if(i!=j)
            {
                if (tmp[i][1]==tmp[j][1]&&tmp[i][2]==tmp[j][2])
                {
                    if(atof(tmp[i][3].c_str())>atof(tmp[j][3].c_str()))
                    {
                        tmp.erase(tmp.begin()+i);
                    }
                }
            }
    }
    fin.close();
    fstream fout( "output.txt", (ios::app | ios::out) );
    for (int i = 0 ; i < tmp.size() ; i++)
    {
        fout<<tmp[i][0]<<"_"<<tmp[i][1]<<"_"<<tmp[i][2]<<" ";
        fout<<tmp[i][0]<<"_"<<tmp[i][1]<<"_"<<tmp[i][2]<<" ";
        
        if (tmp[i][3].size()>3)
            fout<<tmp[i][3][0]<<tmp[i][3][1]<<tmp[i][3][2]<<tmp[i][3][3];
        else
            fout<<tmp[i][3];
        fout<<endl;
    }
    fout.close();
}

