#ifndef POSIXPP_UTILITY
#define POSIXPP_UTILITY

inline uint32_t hton(uint32_t val);
inline  int32_t hton( int32_t val);

inline uint16_t hton(uint16_t val);
inline  int16_t hton( int16_t val);


inline uint32_t ntoh(uint32_t val);
inline  int32_t ntoh( int32_t val);

inline uint16_t ntoh(uint16_t val);
inline  int16_t ntoh( int16_t val);

//******************
//**Implementation**
//******************
inline uint32_t hton(uint32_t val)
{
	return htonl(val);
}
inline int32_t hton(int32_t val)
{
	return (int32_t) htonl((uint32_t) val);
}

inline uint16_t hton(uint16_t val)
{
	return htons(val);
}
inline int16_t hton(int16_t val)
{
	return (int16_t) htons((uint16_t) val);
}


inline uint32_t ntoh(uint32_t val)
{
	return ntohl(val);
}
inline int32_t ntoh(int32_t val)
{
	return (int32_t) ntohl((uint32_t) val);
}

inline uint16_t ntoh(uint16_t val)
{
	return ntohs(val);
}
inline int16_t ntoh(int16_t val)
{
	return (int16_t) ntohs((uint16_t) val);
}
#endif /*POSIXPP_UTILITY*/
