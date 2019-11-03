#ifndef COREINTERFACE_H_
#define COREINTERFACE_H_

class CoreInterface
{
public:
	virtual ~CoreInterface() {}

	virtual void launch() const = 0;

	virtual void stop() const = 0;
};

#endif
