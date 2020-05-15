#pragma once
#include <Cappuccino.h>

class SentryBot : public Capp::GameObject {
public:

	SentryBot();
	~SentryBot();
	
	void update() override;
	void onEvent(Capp::Event& e) override;

private:

	bool scaleUp = true;
	
};