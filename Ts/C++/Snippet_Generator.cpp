#include <bits/stdc++.h>

using namespace std;

#define elif else if

string escape_for_json(string s){
    if(!s.empty() && s.back()=='\r'){
        s.pop_back();
    }
    string ret;
    for(char c: s){
        if(c=='\\'){
            ret+="\\\\";
        } elif(c=='\"'){
            ret+="\\\"";
        } elif(c=='\t'){
            ret+="\\t";
        } else{
            ret+=c;
        }
    }
    return ret;
}

string quote_for_json(string s){
    return "\""+escape_for_json(s)+"\"";
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    string name, scope, prefix, description;
    getline(cin, name);
    getline(cin, scope);
    getline(cin, prefix);
    getline(cin, description);
    vector<string> body;
    for(string line; getline(cin, line);){
        body.push_back(line);
    }

    cout<<quote_for_json(name)<<": {\n";
    cout<<'\t'<<"\"scope\": "<<quote_for_json(scope)<<",\n";
    cout<<'\t'<<"\"prefix\": "<<quote_for_json(prefix)<<",\n";
    cout<<'\t'<<"\"body\": "<<"[\n";
    for(int i(0); i<body.size(); ++i){
        cout<<"\t\t"<<quote_for_json(body[i])<<((i==body.size()-1)?"":",")<<'\n';
    }
    cout<<'\t'<<"],\n";
    cout<<'\t'<<"\"description\": "<<quote_for_json(description)<<'\n';
    cout<<"}";

    return 0;
}