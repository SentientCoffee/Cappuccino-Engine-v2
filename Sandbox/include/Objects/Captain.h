#pragma once
#include <Cappuccino.h>

class Captain : public Capp::GameObject {
public:

	Captain();
	~Captain();

	void update() override;
	void onEvent(Capp::Event& e) override;
	
};
