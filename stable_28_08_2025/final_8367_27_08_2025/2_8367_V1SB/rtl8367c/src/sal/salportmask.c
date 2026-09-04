#include <salportmask.h>

#if SAL_ENABLE
extern uint16 phyAllPortMask;

void
sal_pPortMask_addAll (
	sal_pPortmask_t *pPPortMask
	)
{
	int	i;

	pPPortMask->bits[0] = 0;

	for (i = 0; i < SAL_PORT_ID_CPU; ++i) {
		pPPortMask->bits[0] |= (1UL << i);
	}
}

void 
sal_pPortMask_delAll(
    sal_pPortmask_t *pPPortMask
    )
{

	pPPortMask->bits[0] = 0;
}


void 
sal_pPortMask_addPort(
    sal_pPortmask_t *pPPortMask, 
    sal_port_t port
    )
{
//	if (port > SAL_MAX_PHY_N_CPU_PORT - 1) {
//		return;
//	}

	pPPortMask->bits[0]	|= (1UL << port);
}
#if 0
void 
sal_pPortMask_delPort(
    sal_pPortmask_t *pPPortMask, 
    sal_port_t port
    )
{
	if (port > SAL_MAX_PHY_N_CPU_PORT - 1) {
		return;
	}

	pPPortMask->bits[0]	&= ~(1UL << port);
}
#endif

void 
sal_pPortMask_addPorts(
    sal_pPortmask_t *pPPortMask, 
    sal_pPortmask_t *pPPortMaskPorts
    )
{

	pPPortMask->bits[0] |= pPPortMaskPorts->bits[0];
}

void 
sal_pPortMask_delPorts(
    sal_pPortmask_t *pPPortMask, 
    sal_pPortmask_t *pPPortMaskPorts
    )
{

	pPPortMask->bits[0] &= ~(pPPortMaskPorts->bits[0]);
}

int32 
sal_pPortMask_chkPort(
    sal_pPortmask_t *pPPortMask, 
    sal_port_t port
)
{
//	if (port > SAL_MAX_PHY_N_CPU_PORT - 1) {
//		return SAL_ERR_FAILED;
//	}

#if defined(LWS_8G_2F) || defined(LWS_5G_1F)
        if(port > SAL_PORT_ID_CPU)
            return SAL_ERR_FAILED;
#elif defined(LWS_5G) || defined(LWS_8G)
          if (port > SAL_MAX_PHY_PORT) {
              return SAL_ERR_FAILED;
          }
#elif defined(LWS_8G)
#else
#endif

	if (pPPortMask->bits[0] & (1UL << port))
	{
		return SAL_ERR_SUCCESS;
	}
	else {
		return SAL_ERR_FAILED;
	}
}

void 
sal_pPortMask_copy(
    sal_pPortmask_t *pPPortMask, 
    sal_pPortmask_t *pPPortMaskSrc
    )
{

	pPPortMask->bits[0] = pPPortMaskSrc->bits[0];
}

void 
sal_pPortMask_copyLMask (
    sal_pPortmask_t *pPPortMask, 
    sal_lPortmask_t *pLPortMask
    )
{
	int	i;

	pPPortMask->bits[0] = 0;
#if defined(LWS_8G_2F)
	for (i = 0; i < SAL_MAX_PHY_PORT; ++i) {
		if (pLPortMask->bits[0]	& (1UL << i))
		{
		    if(i == (SAL_MAX_PHY_PORT -2)){
		        pPPortMask->bits[0]	|= (1UL << EXT_PORT0);
		    }
		    else if(i == SAL_MAX_PHY_PORT -1){
		        pPPortMask->bits[0]	|= (1UL << EXT_PORT1);
		    }
		    else{
			    pPPortMask->bits[0]	|= (1UL << i);
			}
		}
	}
	if (pLPortMask->bits[0]	& (1UL << EXT_PORT0))
	    pPPortMask->bits[0] |= 1UL << EXT_PORT0;
	    
	if (pLPortMask->bits[0]	& (1UL << EXT_PORT1))
	    pPPortMask->bits[0] |= 1UL << EXT_PORT1;
#elif defined(LWS_5G_1F)
	for (i = 0; i < SAL_MAX_PHY_PORT; ++i) {
		if (pLPortMask->bits[0]	& (1UL << i))
		{
		    if(i == 5){
		        pPPortMask->bits[0]	|= (1UL << EXT_PORT0);
		    }
		    else{
			    pPPortMask->bits[0]	|= (1UL << i);
			}
		}
	}    
	
	if (pLPortMask->bits[0]	& (1UL << EXT_PORT0))
	    pPPortMask->bits[0] |= 1UL << EXT_PORT0;
#else
    for (i = 0; i < SAL_MAX_PHY_N_CPU_PORT; ++i) {
        if (pLPortMask->bits[0] & (1UL << i))
        {
            pPPortMask->bits[0] |= (1UL << i);
        }
    }
#endif
	
}
#if 0
void 
sal_pPortMask_copyUL(
    sal_pPortmask_t *pPPortMask, 
    uint32          *pMask
    )   
{
	pPPortMask->bits[0] = *pMask;
}

#endif

void
sal_pPortMask_toUL (
	uint32 *pMask, 
	sal_pPortmask_t *pPPortMask
	)
{
	*pMask = pPPortMask->bits[0];
}

void 
sal_pPortMask_and(
    sal_pPortmask_t *pPPortMask, 
    sal_pPortmask_t *pPPortMaskA, 
    sal_pPortmask_t *pPPortMaskB
    )
{

	pPPortMask->bits[0]	= pPPortMaskA->bits[0] & pPPortMaskB->bits[0];
}

void
sal_pPortMask_or (
	sal_pPortmask_t *pPPortMask, 
	sal_pPortmask_t *pPPortMaskA, 
	sal_pPortmask_t *pPPortMaskB
	)
{

	pPPortMask->bits[0]	= pPPortMaskA->bits[0] | pPPortMaskB->bits[0];
}

#if 0
void 
sal_pPortMask_xor(
    sal_pPortmask_t *pPPortMask, 
    sal_pPortmask_t *pPPortMaskA, 
    sal_pPortmask_t *pPPortMaskB
)
{

		pPPortMask->bits[0]	= pPPortMaskA->bits[0] ^ pPPortMaskB->bits[0];
}

int32 
sal_pPortMask_cmp(
    sal_pPortmask_t *pPPortMaskA, 
    sal_pPortmask_t *pPPortMaskB
    )
{

		if (pPPortMaskA->bits[0] != pPPortMaskB->bits[0]) {
			return SAL_ERR_FAILED;
		}

	return SAL_ERR_SUCCESS;
}
#endif

int32 
sal_pPortMask_isZero (
    sal_pPortmask_t *pPPortMask
    )
{

		if (pPPortMask->bits[0]) {
			return SAL_ERR_FAILED;
		}

	return SAL_ERR_SUCCESS;
}

void 
sal_lPortMask_addAll(
    sal_lPortmask_t *pLPortMask
    )
{
	int	i;

	pLPortMask->bits[0] = 0;

#if defined(LWS_8G_2F)
	for (i = 0; i < 8; ++i) {
		pLPortMask->bits[0] |= (1UL << i);
	} 		
	pLPortMask->bits[0] |= (1UL << EXT_PORT0);
	pLPortMask->bits[0] |= (1UL << EXT_PORT1);
#elif defined(LWS_5G_1F)
	for (i = 0; i < 5; ++i) {
		pLPortMask->bits[0] |= (1UL << i);
	} 		
	pLPortMask->bits[0] |= (1UL << EXT_PORT0);    	
#else
	for (i = 0; i < SAL_MAX_PHY_PORT; ++i) {
		pLPortMask->bits[0] |= (1UL << i);
	}
#endif
}

void 
sal_lPortMask_delAll (
    sal_lPortmask_t *pLPortMask
    )
{

	pLPortMask->bits[0] = 0;
}

void 
sal_lPortMask_addPort (
    sal_lPortmask_t *pLPortMask, 
    sal_port_t port
    )
{
	if (port > SAL_PORT_ID_CPU - 1) {
		return;
	}

	pLPortMask->bits[0]	|= (1UL << port);
}

void 
sal_lPortMask_delPort(
    sal_lPortmask_t *pLPortMask, 
    sal_port_t port
    )
{
	if (port > SAL_PORT_ID_CPU - 1) {
		return;
	}

	pLPortMask->bits[0] &= ~(1UL << port);
}
#if 1
void
sal_lPortMask_addPorts (
	sal_lPortmask_t *	pLPortMask,
	sal_lPortmask_t *	pLPortMaskPorts
	)
{

	pLPortMask->bits[0] |= pLPortMaskPorts->bits[0];
}
#endif

void 
sal_lPortMask_delPorts(
    sal_lPortmask_t *pLPortMask, 
    sal_lPortmask_t *pLPortMaskPorts
    )
{

	pLPortMask->bits[0] &= ~(pLPortMaskPorts->bits[0]);
}

int32 
sal_lPortMask_chkPort(
    sal_lPortmask_t *pLPortMask, 
    sal_port_t port
    )
{
//	if (port > (SAL_MAX_PHY_N_CPU_PORT + SAL_MAX_TRK_PORT) - 1) {
//		return SAL_ERR_FAILED;
//	}

#if defined(LWS_8G_2F)
    if(port > 17)
        return SAL_ERR_FAILED;
#elif defined(LWS_5G_1F)
    if(port > 16)
        return SAL_ERR_FAILED;
#elif defined(LWS_5G)
      if (port > (SAL_MAX_PHY_N_CPU_PORT + SAL_MAX_TRK_PORT) - 1) {
          return SAL_ERR_FAILED;
      }
#elif defined(LWS_8G)
      if (port > (SAL_MAX_PHY_N_CPU_PORT + SAL_MAX_TRK_PORT) - 1) {
          return SAL_ERR_FAILED;
      }
#else
#endif

	if (pLPortMask->bits[0]	& (1UL << port))
	{
		return SAL_ERR_SUCCESS;
	}
	else {
		return SAL_ERR_FAILED;
	}
}

#if 0//NOT used
void 
sal_lPortMask_copy(
    sal_lPortmask_t *pLPortMask, 
    sal_lPortmask_t *pLPortMaskSrc
    )
{

	pLPortMask->bits[0] = pLPortMaskSrc->bits[0];
}
#endif

void 
sal_lPortMask_copyPMask(
    sal_lPortmask_t *pLPortMask, 
    sal_pPortmask_t *pPPortMask
    )
{

	pLPortMask->bits[0] = pPPortMask->bits[0];
}

void 
sal_lPortMask_copyUL(
    sal_lPortmask_t *pLPortMask, 
    uint32 *pMask
    )
{
	pLPortMask->bits[0] = *pMask;
}

#if 0//NOT used

void 
sal_lPortMask_toUL (
    uint32 *pMask, 
    sal_lPortmask_t *pLPortMask
    )
{
	*pMask = pLPortMask->bits[0];
}

void 
sal_lPortMask_and(
    sal_lPortmask_t *pLPortMask, 
    sal_lPortmask_t *pLPortMaskA, 
    sal_lPortmask_t *pLPortMaskB
    )
{

	pLPortMask->bits[0]	= pLPortMaskA->bits[0] & pLPortMaskB->bits[0];
}

void sal_lPortMask_or(
    sal_lPortmask_t *pLPortMask, 
    sal_lPortmask_t *pLPortMaskA, 
    sal_lPortmask_t *pLPortMaskB
    )
{

	pLPortMask->bits[0]	= pLPortMaskA->bits[0] | pLPortMaskB->bits[0];
}

void 
sal_lPortMask_xor(
    sal_lPortmask_t *pLPortMask, 
    sal_lPortmask_t *pLPortMaskA, 
    sal_lPortmask_t *pLPortMaskB
    )
{

	pLPortMask->bits[0]	= pLPortMask->bits[0] ^ pLPortMaskB->bits[0];
}
#endif
/*KevinLee: get result of A deletes B*/

#if 0
void sal_lPortMask_delPorts2 (
	sal_lPortmask_t *	pLPortMask,
	sal_lPortmask_t *	pLPortMaskA,
	sal_lPortmask_t *	pLPortMaskB
	)
{

	pLPortMask->bits[0]	= pLPortMaskA->bits[0] & (~ pLPortMaskB->bits[0]);
}
#endif

#if 0
int32 
sal_lPortMask_cmp(
    sal_lPortmask_t *pLPortMaskA, 
    sal_lPortmask_t *pLPortMaskB
    )
{

	if (pLPortMaskA->bits[0] != pLPortMaskB->bits[0]) {
		return SAL_ERR_FAILED;
	}

	return SAL_ERR_SUCCESS;
}
#endif

int32 
sal_lPortMask_isZero(
    sal_lPortmask_t *pLPortMask
    )
{

	if (pLPortMask->bits[0]) {
		return SAL_ERR_FAILED;
	}

	return SAL_ERR_SUCCESS;
}


#endif
