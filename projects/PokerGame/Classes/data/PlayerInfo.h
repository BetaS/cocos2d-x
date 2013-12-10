#pragma once

#include <string>

class PlayerInfo
{
public:
	PlayerInfo(void);
	~PlayerInfo(void);

private:
	std::string name;

public:
	void setPlayerName(std::string name) { this->name = name; }
	std::string getPlayerName() { return name; }

private:
	int nMoney;

public:
	void setMoney(int money) { nMoney = money; }
	int getMoney() { return nMoney; }

private:
	int nBetting;

	int nLastBet;
	int nLastOpt;

public:
	void betting(int opt, int bet) { nLastOpt=opt; nLastBet=bet; nBetting+=bet; nMoney-=bet; }
	void setBetting(int bet) { nLastBet=bet; nBetting = bet; }
	int getBetting() { return nBetting; }

	int getLastOption() { return nLastOpt; }
	int getLastBetting() { return nLastBet; }
};

