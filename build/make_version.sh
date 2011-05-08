cat > ../include/version.h <<-CPP
	#ifndef MEDIA_VERION_NUMBER
	#define MEDIA_VERION_NUMBER "`git describe`"
	#endif 