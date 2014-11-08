#ifndef _NONCOPYABLE_H_
#define _NONCOPYABLE_H_

namespace cpp_ns
{

class NonCopyable
{
protected:
	NonCopyable() {}
	~NonCopyable() {}

private:
	NonCopyable(const NonCopyable&);
	void operator=(const NonCopyable&);
};

} // namespace cpp_ns

#endif // _NONCOPYABLE_H_
