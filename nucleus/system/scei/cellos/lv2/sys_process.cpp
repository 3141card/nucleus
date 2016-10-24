/**
 * (c) 2014-2016 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#include "sys_process.h"
#include "nucleus/system/scei/cellos/lv2.h"
#include "nucleus/logger/logger.h"
#include "nucleus/emulator.h"

namespace sys {

S32 sys_process_getpid() {
    LV2& lv2 = static_cast<LV2&>(*nucleus.sys.get());

    return 0x01000500; // TODO
}

S32 sys_process_getppid() {
    LV2& lv2 = static_cast<LV2&>(*nucleus.sys.get());

    return 0x01000300; // TODO
}

S32 sys_process_exit(S32 errorcode) {
    LV2& lv2 = static_cast<LV2&>(*nucleus.sys.get());

    nucleus.task(NUCLEUS_EVENT_STOP);
    return CELL_OK;
}

S32 sys_process_get_paramsfo(U08* buffer) {
    LV2& lv2 = static_cast<LV2&>(*nucleus.sys.get());

    return CELL_OK;
}

S32 sys_process_get_sdk_version(U32 pid, BE<U32>* version) {
    LV2& lv2 = static_cast<LV2&>(*nucleus.sys.get());

    if (!version) {
        return CELL_EFAULT;
    }
    *version = lv2.proc.param.sdk_version;
    return CELL_OK;
}

S32 sys_process_is_spu_lock_line_reservation_address(U32 addr, U64 flags) {
    logger.warning(LOG_HLE, "LV2 Syscall (0x00E) called: sys_process_is_spu_lock_line_reservation_address");
    return CELL_OK;
}

/***********************************************************************
* SC 0x002(002):
* 
* uint32_t *child_proc_id       = (IN/OUT) variable to hold child process ID
* uint32_t *status              = (IN) variable to hold
* uint32_t flag                 = (IN) 0(?) or 1(?)
***********************************************************************/
S32 sys_process_wait_for_child(BE<U32>* child_proc_id, BE<U32>* status, U32 flag)
{
	return CELL_OK; // TODO...
}


/***********************************************************************
* SC 0x004(004):
* 
* uint32_t proc_id              = (IN) process ID
* return: status of the process for the given ID
***********************************************************************/
S32 sys_process_get_status(U32 proc_id)
{
	return CELL_OK; // TODO...
}

/***********************************************************************
* SC 0x005(005):
* 
***********************************************************************/
S32 sys_process_detach_child(U32 child_proc_id)
{
	return CELL_OK; // TODO...
}

/***********************************************************************
* SC 0x00C(012):
* 
* uint32_t obj_type             = (IN) type of object to get the count for
* uint32_t *count               = (IN) variable to hold the object count 
***********************************************************************/
S32 sys_process_get_number_of_object(U32 obj_type, BE<U32>* count)
{
	return CELL_OK; // TODO...
}

/***********************************************************************
* SC 0x00D(013): get SYS_SPUPORT_OBJECT IDs denied!!!
* 
* uint32_t obj_type             = (IN) type of object to get IDs for
* void *id_list                 = (IN) buffer to hold object IDs
* uint32_t id_list_size         = (IN) size of buffer in byte
* uint32_t *count               = (OUT) count of IDs copied out
***********************************************************************/
S32 sys_process_get_id(U32 obj_type, BE<U32>* id_list, U32 id_list_size, BE<U32>* count)
{
	return CELL_OK; // TODO...
}

/***********************************************************************
* SC 0x013(019):
* 
* uint32_t proc_id              = (IN) ID of process to kill
***********************************************************************/
S32 sys_process_kill(U32 proc_id)
{
	return CELL_OK; // TODO...
}

/***********************************************************************
* SC 0x015(021):
* 
* uint32_t *proc_id             = (OUT) variable to hold the process ID
* int32_t primary_prio          = (IN) priority of the process primary thread
* uint32_t flags                = (IN)
* void *stack                   = (IN) argv/envp
* uint32_t stack_size           = (IN) argv/envp size in byte
* uint64_t intr_mask            = (IN) process interrupt bitmap mask, optional
* uint32_t trace_id             = (IN) process debugging trace ID, optional
***********************************************************************/
S32 sys_process_spawn(BE<U32>* proc_id, S32 primary_prio, U32 flags, BE<U32>* stack, U32 stack_size, U64 intr_mask, U32 trace_id)
{
	return CELL_OK; // TODO...
}

/***********************************************************************
* SC 0x016(022):
* 
* int32_t exit_status           = (IN) process exit status
* void *user_data               = (IN) buffer with user data, parsed on exit
* uint32_t user_data_size       = (IN) user data size in byte
***********************************************************************/
S32 sys_process_exit2(S32 exit_status, BE<U32>* user_data, U32 user_data_size)
{
	return CELL_OK; // TODO...
}

/***********************************************************************
* SC 0x017(023):
* 
***********************************************************************/
S32 sys_process_wait_for_child2(BE<U32>* child_proc_id, BE<U32>* status, BE<U32>* data_out, U32 data_out_size, U32 arg_5, U32 flags)
{
	return CELL_OK; // TODO...
}

/***********************************************************************
* SC 0x018(024):
* 
* uint32_t *proc_id             = (OUT) variable to hold the process ID
* int32_t primary_prio          = (IN) priority of the process primary thread
* uint32_t flags                = (IN)
* void *stack                   = (IN) argv/envp
* uint32_t stack_size           = (IN) argv/envp size in byte
* uint64_t intr_mask            = (IN) process interrupt bitmap mask, optional
* uint32_t trace_id             = (IN) process debugging trace ID, optional
* uint32_t mc_id                = (IN) memory container ID, process user space
***********************************************************************/
S32 sys_process_spawns_a_self(BE<U32>* proc_id, S32 primary_prio, U32 flags, BE<U32>* stack, U32 stack_size, U64 proc_intr_mask, U32 trace_id, U32 mc_id)
{
	return CELL_OK; // TODO...
}


/***********************************************************************
* SC 0x01A(026):
* 
* int32_t exit_status           = (IN) process exit status
* void *user_data               = (IN) buffer with user data, parsed on exit
* uint32_t user_data_size       = (IN) user data size in byte
* uint32_t flags                = (IN) 
***********************************************************************/
S32 sys_process_exit3(S32 exit_status, BE<U32>* user_data, U32 user_data_size, U32 flags)
{
	return CELL_OK; // TODO...
}

/***********************************************************************
* SC 0x01B(027):
* 
* uint32_t *proc_id             = (OUT) variable to hold the process ID
* int32_t primary_prio          = (IN) priority of the process primary thread
* uint32_t flags                = (IN)
* void *stack                   = (IN) argv/envp
* uint32_t stack_size           = (IN) argv/envp size in byte
* uint32_t mc_id                = (IN) memory container ID, process user space
* lv2_proc_param_sfo *param_sfo = (IN) process params
* lv2_proc_dbg *dbg_data        = (IN) struct with debugging values
***********************************************************************/
S32 sys_process_spawns_a_self2(BE<U32>* proc_id, S32 primary_prio, U32 flags, BE<U32>* stack, U32 stack_size, U32 mc_id, sys_param_sfo_t* param_sfo, sys_process_dbg_t* dbg_data)
{
	return CELL_OK; // TODO...
}

/***********************************************************************
* SC 0x01A(026):
* 
* uint32_t obj_type             = (IN) type of object to get the count for
***********************************************************************/
S32 sys_process_get_number_of_object2(U32 obj_type)
{
	return CELL_OK; // TODO...
}

/***********************************************************************
* SC 0x01D(029): get SYS_SPUPORT_OBJECT IDs allowed!!!
* 
* uint32_t obj_type             = (IN) type of object to get IDs for
* void *id_list                 = (IN) buffer to hold object IDs
* uint32_t id_list_size         = (IN) size of buffer in byte
* uint32_t *count               = (OUT) count of IDs copied out
***********************************************************************/
S32 sys_process_get_id2(U32 obj_type, BE<U32>* id_list, U32 id_list_size, BE<U32>* count)
{
	return CELL_OK; // TODO...
}


/***********************************************************************
* SC 0x01F(031):
* 
* return: ppu_guid of current process 
***********************************************************************/
S32 sys_process_get_ppu_guid() {
	
	return CELL_OK; // TODO...
}

}  // namespace sys
