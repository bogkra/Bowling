#include "bowling.hpp"
#include <cctype>
#include <numeric>
#include <algorithm>

using namespace std;

void Bowling::eliminateDoubleSeparator(Frames & frames)
{
  if (frames.size() > 11) 
    frames.erase(frames.begin() + 10);
}


void Bowling::frameize(const string& s)
{
  Frame s0 = s;
  Frame::size_type pos;
  while ((pos = s0.find(Symbol::separator)) != Frame::npos) 
  {  
    frames.push_back(s0.substr(0,pos));
    s0 = s0.substr(pos+1, s0.length());  
  }
  frames.push_back(s0);
  eliminateDoubleSeparator(frames);
}


Bowling::Bowling(const string& s)
{
  frameize(s);
  validateFrames();
}


bool inside(const string & s, const char ch)
{
  return s.find(ch) != std::string::npos;
}


bool Bowling::isSymbolCorrect(const char ch)
{
  return inside(Symbol::correctSymbols, ch);
}


bool Bowling::areSymbolsCorrect(const Frame& frame)
{
  return all_of(frame.begin(), frame.end(), 
           [=](const char symbol) 
           { 
             return isSymbolCorrect(symbol);
           });
}


bool Bowling::isFrameSizeCorrect(const Frame& frame)
{
  return frame.size() <= 2 and frame.size() > 0 ;
}


bool Bowling::validateFrames()
{
  return all_of(frames.begin(), prev(frames.end()),
           [=](const Frame & frame) 
           {
             return isFrameSizeCorrect(frame) and areSymbolsCorrect(frame) and (frame.size()!=2 or sumAPair(frame)<=10);
           } );       
}


int Bowling::translateChar(const char ch)
{
  if (ch==Symbol::miss or ch==Symbol::spare)
    return 0;
  else if (ch == Symbol::strike)
    return 10;
  else
    return ch - '0';
}


bool Bowling::hasStrike(const Frame& frame)
{
  return frame[0] == Symbol::strike;
}


bool Bowling::hasSpare(const Frame& frame)
{
  return frame.length() == 2 and frame[1] == Symbol::spare;
}


int Bowling::scoreFor2Balls(const Iterator & it)
{
  return it->length() == 2  ?  sumAPair(*it)  :  10 + translateChar((*next(it))[0]); 
}


int Bowling::countExtra(const Iterator & it)  
{
  const Frame frame = *it;
  const Frame nextFrame = *next(it);

  if (hasSpare(frame))
    return translateChar(nextFrame[0]);
  else if (hasStrike(frame)) 
    return scoreFor2Balls(next(it));
  else return 0; 
}


int Bowling::countExtras()  
{  
  Iterator it = frames.begin();
  return accumulate(it, prev(frames.end()), 0, 
              [&](int & sum, const Frame & frame) 
              { 
                return sum + countExtra(it++);
                if (frame == "") {};  
              });
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
  return accumulate(frames.begin(), prev(frames.end()), 0, 
              [=](int & sum, const Frame & frame) 
              { 
                return sum + countSeparateScore(frame);
              });
}


int Bowling::countScore()
{
  if (validateFrames())
    return countStandardScore() + countExtras();
  else 
    throw std::invalid_argument("Invalid input!");
}
