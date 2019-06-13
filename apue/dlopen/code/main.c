#include <stdio.h>
#include <errno.h>
#include <dlfcn.h>

int	main( void )
{
	void*	handle = dlopen( "./libfun.so", RTLD_LAZY ) ;
	if ( !handle )
	{
		perror( "dlopen " ) ;
		return	-1 ;
	}

	dlerror() ;

	void(*func)(void) = dlsym( handle, "func" ) ;
	if ( dlerror() != NULL )
	{
		perror( "dlsym " ) ;
		return	-1 ;
	}

	func() ;

	dlclose( handle ) ;

	return	0 ;
}
