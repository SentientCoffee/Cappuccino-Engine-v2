#pragma once
#include <Cappuccino.h>

class F16 : public Capp::GameObject {
public:

	F16();
	~F16();

	void update() override;
	void onEvent(Capp::Event& e) override;
	
};
