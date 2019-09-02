#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#ifdef _WIN32
	#define dirSep "\\"
#else
	#define dirSep "/"
#endif

#ifdef __CUDACC__
	#define CUDA_HOST_DEVICE __host__ __device__
	#include <cuda.h>
#else
	#define CUDA_HOST_DEVICE
#endif

/// \defgroup GlobalDefinitions Définitions globales
/// \brief Ensemble des paramètres globaux de l'application.
/// @{

/// \brief Paramètre de verbosité.
extern int verbose;

/// \brief Type pour les nombres réels.
typedef float real;
/// @}

#endif
