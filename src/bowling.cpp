#include "bowling.hpp"
#include <iostream>
#include <cctype>

using namespace std;

void Bowling::frameize(const Frame& s)
{
  Frame s0 = s;
  Frame::size_type pos;
  while ((pos = s0.find(separator)) != Frame::npos) {  
    frames.push_back(s0.substr(0,pos));
    s0 = s0.substr(pos+1, s0.length());  
  }
  frames.push_back(s0);
  if (frames.size() > 11) 
    frames.erase(frames.begin() + 10);
}


Bowling::Bowling(const Frame& s)
{
  frameize(s);
  validateFrames();
}


bool Bowling::correctSymbol(const char symbol)
{
  return symbol==miss or symbol==spare or symbol==strike or symbol==separator or (isdigit(symbol) and symbol!='0') ;
}

bool Bowling::correctSymbols(const Frame& frame)
{
  for(char symbol : frame)
     if (not correctSymbol(symbol))
        return false;
  return true;
}

bool isFrameSizeIncorrect(const Frame& frame)
{
  return frame.size() > 2 or frame.size() == 0 ;
}

Frames butLast(const Frames& fr)
{
  auto result(fr);
  result.pop_back();
  return result;
}

bool Bowling::validateFrames()
{
    for(Frame frame : standardFrames())
        if (isFrameSizeIncorrect(frame) or not correctSymbols(frame) or (frame.size()==2 and sumAPair(frame)>10))
            return false;
    return (correctSymbols(bonusFrame())); 
}

int Bowling::translateChar(const char ch)
{
  if (ch==miss or ch==spare)
    return 0;
  else if (ch==strike)
    return 10;
  else
    return ch - '0';
}

bool Bowling::hasStrike(const Frame& frame)
{
  return frame[0] == strike;
}

bool Bowling::hasSpare(const Frame& frame)
{
  return frame.length() == 2 and frame[1] == spare;
}


int Bowling::scoreFor2Balls(const int i)
{
  return frames.at(i).length() == 2  ?  sumAPair(frames.at(i))  :  10 + translateChar(frames.at(i+1)[0]); 
}

int Bowling::countExtra(const int i)  
{
  if (hasSpare(frames.at(i)))
    return translateChar(frames.at(i+1)[0]);
  else if (hasStrike(frames.at(i))) 
    return scoreFor2Balls(i+1);
  return 0; 
}

int Bowling::countExtras()  
{  
  int extraScore = 0;
  int i = 0;
  for(auto frame : standardFrames()) 
    extraScore += countExtra(i++);
  return extraScore;
}  


int Bowling::sumAPair(const Frame& frame)
{
   return  hasSpare(frame)  ?  10  :  translateChar(frame[0]) + translateChar(frame[1]);                
}


int Bowling::countSeparateScore(const Frame& frame)
{
  return  frame.length() == 1  ?   10  :  sumAPair(frame);
}


int Bowling::countStandardScore()
{  
  int score = 0;  
  for(auto frame: standardFrames()) 
    score += countSeparateScore(frame);
  return score;
}

int Bowling::countScore()
{
  if (validateFrames())
     return countStandardScore() + countExtras();
  else 
     return -1;
}
