#ifndef __HELPER_PTZ_H__
#define __HELPER_PTZ_H__

#include "soapH.h"
#include "soapStub.h"

#include "global.h"

struct tt__PTZConfiguration *helper_create_default_ptz_configuration(struct soap *soap);

struct tt__Space2DDescription *helper_create_space_2d_description(struct soap *soap);

struct tt__Space1DDescription *helper_create_space_1d_description(struct soap *soap);

struct tt__PTZNode *helper_create_ptz_node(struct soap *soap, int node_index);


// 板级控制函数
void helper_do_ptz_get_pantilt_range(float *x_min, float *x_max, float *y_min, float *y_max);

void helper_do_ptz_get_status(float *x, float *y);




void helper_do_ptz_relative_move(float delta_x, float delta_y);

void helper_do_ptz_absolute_move(float x, float y);




#endif // __HELPER_PTZ_H__
