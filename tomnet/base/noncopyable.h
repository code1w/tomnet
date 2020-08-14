#ifndef BASE_NONCOPYABLE_H
#define BASE_NONCOPYABLE_H
class noncopyable {
protected:
	noncopyable() {}
	virtual ~noncopyable() {}
private:
	noncopyable(const noncopyable&) = delete;
	noncopyable& operator=(const noncopyable&) = delete;
	noncopyable(noncopyable&&) = delete;
	noncopyable& operator=(noncopyable&&) = delete;
};

#endif //  TOM_NET_NONCOPYABLE_H