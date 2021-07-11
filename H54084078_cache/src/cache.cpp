#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
using namespace std;

unsigned int addressDecimal;

unsigned int TurnAddress(string address)
{
    stringstream str;
    str << hex << address;
    str >> addressDecimal;
    return addressDecimal;
}

struct data
{
    bool valid = false;
    int time = 0;
    unsigned int tag = 0;
};

int main(int argc, char *argv[])
{

    fstream fileIn, fileOut;
    fileIn.open(argv[1], ios::in);
    fileOut.open(argv[2], ios::out);
    int _cache_size, _block_size, _block, _associative, _replace, _tag_bit, _index_bit, _offest, _index, _time = 0, _rtime, _rindex;
    unsigned int datadata, _tag;

    bool victim;
    string address;
    fileIn >> _cache_size >> _block_size >> _associative >> _replace;
    //_block_size=pow(2,_offest);
    _offest = log2(_block_size); //2的_offset次方=_block_size
    _block = _cache_size * 1024 / _block_size;

    data cache[_block];

    switch (_associative)
    {
    case 0:                        //direct-mapped
        _index_bit = log2(_block); //2的_index_bit次方=_block
        _tag_bit = 32 - _index_bit - _offest;

        while (fileIn >> address)
        {
            datadata = TurnAddress(address);
            _time += 1;
            unsigned int shift = datadata << _tag_bit;
            _index = (shift) >> (_tag_bit + _offest);
            _tag = datadata >> (_index_bit + _offest);
            if (_replace == 2)
            {
                _rtime = 0;
            }
            else if (_replace == 1)
            {
                _rtime = 100000000;
            }
            else if (_replace == 0)
            {
                _rtime = 100000000;
            }
            victim = false;
            if (cache[_index].valid == 1 && cache[_index].tag == _tag)
            {
                if (_replace == 0)
                {
                    cache[_index].time = cache[_index].time; //unchanged
                }
                if (_replace == 1)
                {
                    cache[_index].time = _time;
                }
                if (_replace == 2)
                {
                    cache[_index].time = cache[_index].time; //unchanged
                }
                fileOut << -1 << endl;
                victim = true;
            }
            if (victim == false)
            {
                if (cache[_index].valid == 0)
                {
                    cache[_index].valid = 1;
                    cache[_index].time = _rtime;
                    cache[_index].tag = _tag;
                    fileOut << -1 << endl;
                    victim = true;
                }
            }

            if (victim == false)
            {
                switch (_replace)
                {
                case 0: //FIFO
                {
                    fileOut << cache[_index].tag << endl;
                    cache[_index].time = _time;
                    cache[_index].tag = _tag;
                    break;
                }
                case 1: //LRU
                {

                    if (_rtime > cache[_index].time)
                    {
                        _rtime = cache[_index].time;
                        _rindex = _index;
                        fileOut << cache[_rindex].tag << endl;
                        cache[_rindex].time = _time;
                        cache[_rindex].tag = _tag;
                    }
                    break;
                }
                case 2: //random
                {
                    srand((unsigned)time(0));
                    int randomNumber;

                    /*if (_rtime < cache[_index].time)
                    {
                        _rtime = cache[_index].time;
                        _rindex = _index;
                        fileOut << cache[_rindex].tag << endl;
                        cache[_rindex].time = _time;
                        cache[_rindex].tag = _tag;
                    }*/
                    randomNumber = (rand() % 4) + 0;
                    _rindex = randomNumber;
                    fileOut << cache[_rindex].tag << endl;
                    cache[_rindex].tag = _tag;
                    break;
                }

                default:
                    break;
                }
            }
        }
        break;
    case 1: //four-way associative

        _index_bit = log2(_block / 4);
        _tag_bit = 32 - _index_bit - _offest; //datadata>>(_index_bit+_offest)
        while (fileIn >> address)
        {

            datadata = TurnAddress(address);
            _time = _time + 1;
            unsigned int shift = datadata << _tag_bit;
            _index = (shift) >> (_offest + _tag_bit);
            _tag = datadata >> (_offest + _index_bit);

            /*  if (_replace == 2)
            {
                _rtime = 0;
            }*/
            if (_replace == 1)
            {
                _rtime = 100000000;
            }
            else if (_replace == 0)
            {
                _rtime = 100000000;
            }
            victim = false;
            for (int i = 0; i < 4; i++)
            {
                if (cache[_index * 4 + i].valid == 1 && cache[_index * 4 + i].tag == _tag)
                {
                    if (_replace == 0)
                    {
                        cache[_index * 4 + i].time = cache[_index * 4 + i].time; //unchanged
                    }
                    if (_replace == 1)
                    {
                        cache[_index * 4 + i].time = _time;
                    }
                    if (_replace == 2)
                    {
                        cache[_index * 4 + i].time = cache[_index * 4 + i].time; //unchanged
                    }
                    fileOut << -1 << endl;
                    victim = true;
                }
            }
            if (victim == false)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (cache[_index * 4 + i].valid == 0)
                    {
                        cache[_index * 4 + i].tag = _tag;
                        cache[_index * 4 + i].time = _time;
                        cache[_index * 4 + i].valid = 1;
                        fileOut << -1 << endl;
                        victim = true;
                        break;
                    }
                }
            }
            //above loop failed(replace):
            if (victim == false)
            {
                switch (_replace)
                {
                case 0: //FIFO
                {

                    for (int i = 0; i < 4; i++)
                    {
                        if (_rtime > cache[_index * 4 + i].time)
                        {
                            _rtime = cache[_index * 4 + i].time;
                            _rindex = _index * 4 + i;
                        }
                    }
                    fileOut << cache[_rindex].tag << endl;
                    cache[_rindex].time = _time;
                    cache[_rindex].tag = _tag;
                    break;
                }
                case 1: //LRU
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (_rtime > cache[_index * 4 + i].time)
                        {
                            _rtime = cache[_index * 4 + i].time;
                            _rindex = _index * 4 + i;
                        }
                    }
                    fileOut << cache[_rindex].tag << endl;
                    cache[_rindex].time = _time;
                    cache[_rindex].tag = _tag;
                    break;
                }
                case 2: //random
                {

                    srand((unsigned)time(0));
                    int randomNumber;
                    for (int i = 0; i < 4; i++)
                    {
                        /*if (_rtime < cache[_index * 4 + i].time)
                        {
                            _rtime = cache[_index * 4 + i].time;
                            _rindex = _index * 4 + i;
                        }*/
                        randomNumber = (rand() % 4);
                        // if (randomNumber == 1)
                        //{
                        //randomNumber = 0;

                        _rindex = _index * 4 + randomNumber;
                        //}
                        //
                    }

                    // printf("%d",_rindex);
                    fileOut << cache[_rindex].tag << endl;
                    cache[_rindex].tag = _tag;
                    break;
                }
                default:
                    break;
                }
            }
        }
        break;
    case 2: //fully associative
        //_index_bit=0;
        _tag_bit = 32 - _offest;
        while (fileIn >> address)
        {
            datadata = TurnAddress(address);
            _time = _time + 1;
            //_index=(datadata<<_tag_bit)>>(_offest+_tag_bit);
            _tag = datadata >> _offest;

            if (_replace == 2)
            {
                _rtime = 0;
            }
            else if (_replace == 1)
            {
                _rtime = 100000000;
            }
            else if (_replace == 0)
            {
                _rtime = 100000000;
            }
            victim = false;
            for (int i = 0; i < _block; i++)
            {
                if (cache[i].valid == 1 && cache[i].tag == _tag)
                {
                    if (_replace == 0)
                    {
                        cache[i].time = cache[i].time; //unchanged
                    }
                    if (_replace == 1)
                    {
                        cache[i].time = _time;
                    }
                    if (_replace == 2)
                    {
                        cache[i].time = cache[i].time; //unchanged
                    }
                    fileOut << -1 << endl;
                    victim = true;
                    break;
                }
            }
            if (victim == false)
            {
                for (int i = 0; i < _block; i++)
                {
                    if (cache[i].valid == 0)
                    {
                        cache[i].tag = _tag;
                        cache[i].time = _time;
                        cache[i].valid = 1;
                        fileOut << -1 << endl;
                        victim = true;
                        break;
                    }
                }
            }
            //above loop failed(replace):
            if (victim == false)
            {
                switch (_replace)
                {
                case 0: //FIFO
                {
                    for (int i = 0; i < _block; i++)
                    {
                        if (_rtime > cache[i].time)
                        {
                            _rtime = cache[i].time;
                            _rindex = i;
                        }
                    }
                    fileOut << cache[_rindex].tag << endl;
                    cache[_rindex].time = _time;
                    cache[_rindex].tag = _tag;
                    break;
                }
                case 1: //LRU
                {
                    for (int i = 0; i < _block; i++)
                    {
                        if (_rtime > cache[i].time)
                        {
                            _rtime = cache[i].time;
                            _rindex = i;
                        }
                    }
                    fileOut << cache[_rindex].tag << endl;
                    cache[_rindex].time = _time;
                    cache[_rindex].tag = _tag;
                    break;
                }
                case 2: //random
                {
                    /* for (int i = 0; i < _block; i++)
                    {
                        if (_rtime < cache[i].time)
                        {
                            _rtime = cache[i].time;
                            _rindex = i;
                        }
                    }
                    fileOut << cache[_rindex].tag << endl;
                    cache[_rindex].time = _time;
                    cache[_rindex].tag = _tag;*/
                    srand((unsigned)time(0));
                    int randomNumber;
                    _rindex = randomNumber = (rand() % 4) + 0;
                    fileOut << cache[_rindex].tag << endl;
                    cache[_rindex].tag = _tag;
                    break;
                }

                default:
                    break;
                }
            }
        }
        break;
    default:
        break;
    }

    return 0;
}