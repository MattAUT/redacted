#ifndef __TEST_H__
#define __TEST_H__

class Test
{
public:
	Test();
	~Test();

private:

	Test(const Test& p);
	Test& operator=(const Test& p) {}
	
public:
	int fool;
};
#endif // !__TEST_H__

