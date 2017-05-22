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
ngspice::ngspice(string casename)
{
    CaseName = casename ;
}
ngspice::~ngspice()
{
    
}
void ngspice::concat()
{
    
    string a ;
    string input = CaseName.append("_ngspice");
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
    fstream fout("output.txt", ios::app);
    for(int i = 0 ; i<tmp.size();i++)
    {
        fout<<tmp[i][0]<<"_"<<tmp[i][1]<<"_"<<tmp[i][2]<<" ";
        fout<<tmp[i][0]<<"_"<<tmp[i][1]<<"_"<<tmp[i][2]<<" ";
        if(tmp[i][3].size()>3)
            fout<<tmp[i][3][0]<<tmp[i][3][1]<<tmp[i][3][2]<<tmp[i][3][3];
        else
            fout<<tmp[i][3];
        fout<<endl;
    }
    fout.close();
}

