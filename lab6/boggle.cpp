#include "lexicon.h"
#include <iostream>
#include <cstdio>
#include <set>
#include <cstring>
using namespace std;
#define MAX 1000

//棋盘
char board[MAX][MAX];
string currentWord,s;
bool flag;
string word[10000];
bool vis[MAX][MAX];
//这里因为与length相关，所以只能选择这个类型
unsigned long long score,computerScore;
int dir[8][2]={{0,1},{0,-1},{1,0},{-1,0},{1,1},{-1,1},{1,-1},{-1,-1}};
int n;
//新建三个对象
Lexicon english("../EnglishWords.txt");
//这里代表已经有的
//set<string> saveSet;
//这里代表电脑找到的
//set<string> computerSet;

//废了半天劲发现是顺序不对，那就只能用这个了
Lexicon already;
Lexicon computerSet;

void find(int num,int i,int j){
    if(vis[i][j])return;
    if(flag)return;
    if(num==currentWord.length()){
        flag=true;
        return;
    }
    vis[i][j]=true;
    for(auto & x : dir){
        if(i+x[0]>=n||j+x[1]>=n||i+x[0]<0||j+x[1]<0) {
            continue;
        }
        if(board[i+x[0]][j+x[1]]==currentWord[num]  ||  board[i+x[0]][j+x[1]]+32==currentWord[num])
            find(num+1,i+x[0],j+x[1]);
    }
    vis[i][j]=false;
}

void dfs(int i,int j){
    if(i>=n||j>=n||i<0||j<0)return;
    if(vis[i][j]){
        return;
    }
    s+=board[i][j];
    if(!english.containsPrefix(s)){
        s=s.substr(0,s.length()-1);
        return;
    }
    vis[i][j]=true;
    string S=s;
    for(int i = 0;i < s.size();++i){
        S[i] = tolower(s[i]);//str[i] = toupper(s[i]);
    }
    if(english.contains(s) && (!already.contains(s)) && (!computerSet.contains(s))&&s.length()>3){
        computerSet.add(s);
        computerScore+=(s.length()-3);
    }
    for(auto & x : dir){
        dfs(i+x[0],j+x[1]);
    }
    vis[i][j]=false;
    s=s.substr(0,s.length()-1);
}



int main(){
  // TODO
  cin>>n;
  int cnt=0;
  for(int x=0;x<n;x++)scanf("%s",board[x]);
  while(true){
      for(int x=0;x<n;x++)printf("%s\n",board[x]);
      cout<<"Your Score: "<<score<<endl;
      cout<<"Your Words: ";
      for(int x=0;x<cnt;x++)cout<<word[x]<<" ";
      cout<<endl;
      cin>>currentWord;

      if(currentWord!="???"){
          if(currentWord.length()<4){
              cout<<currentWord<<" is too short."<<endl;
              continue;
          }
          if(!english.contains(currentWord)){
              cout<<currentWord<<" is not a word."<<endl;
              continue;
          }
          flag=false;
          for(int x=0;x<n;x++){
              for(int y=0;y<n;y++){
                  //ASCII大小写都可以兼容---如果在棋盘上找到了第一个首字母相符的方块，就启动程序进行搜索
                  if(board[x][y]==currentWord[0] || board[x][y]+32==currentWord[0]) find(1,x,y);
                  if(flag)break;
              }
              if(flag)break;
          }


          if(!flag){
              cout<<currentWord<<" is not on board."<<endl;
          } else{
              //如果不存在并且这个英文单词包含在字典内
              if(!already.contains(currentWord)){
//                  //大小写转换
//                  for(char & x : currentWord){
//                      if(x<'a')x+=32;
//                  }
//                  saveSet.insert(currentWord);
                  already.add(currentWord);
                  int ll=currentWord.length();
                  for(int x=0;x<ll;x++){
                      if(currentWord[x]<'a')currentWord[x]+=32;
                  }
                  word[cnt]=currentWord;
                  cnt++;
                  score+=(currentWord.length()-3);
              }
              else cout<<currentWord<<" is already found."<<endl;
          }
      }
      else{
          for(int x=0;x<n;x++){
              for(int y=0;y<n;y++){
                  dfs(x,y);
              }
          }
          cout<<"Computer Score: "<< computerScore<<endl;
          cout<<"Computer Words: ";
          for(string it:computerSet){
              int l=it.length();
              for(int x=0;x<l;x++)printf("%c",it[x]);
              printf(" ");
          }
          break;
      }
  }
  return 0;
}
