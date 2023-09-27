#if !defined(ENTITY_API)
	#define ENTITY_API /* NOTHING */

	#if defined(WIN32) || defined(WIN64)
		#undef ENTITY_API
		#if defined(entity_EXPORTS)
			#define ENTITY_API __declspec(dllexport)
		#else
			#define ENTITY_API __declspec(dllimport)
		#endif
	#endif // defined(WIN32) || defined(WIN64)

#endif // !defined(ENTITY_API)

