#include "utils/uuid/uuid.h"
#include "config.h"
#include "uuid/uuid.h"
#include "string.h"
#include "utils/log/log.h"

char *uuid_gen(char *prefix)
{
    uuid_t binuuid;
    /*
     * Generate a UUID. We're not done yet, though,
     * for the UUID generated is in binary format
     * (hence the variable name). We must 'unparse'
     * binuuid to get a usable 36-character string.
     */
    uuid_generate_random(binuuid);
    /*
     * uuid_unparse() doesn't allocate memory for itself, so do that with
     * malloc(). 37 is the length of a UUID (36 characters), plus '\0'.
     */
    char *uuid = malloc(get_config().uuid_size);
    uuid_unparse_lower(binuuid, uuid);
    /*
     * Produces a UUID string at uuid consisting of letters
     * whose case depends on the system's locale.
     */
    char *ret_str = (char *)malloc(get_config().uuid_size + strlen(prefix) + 1);
    sprintf(ret_str, "%s_%s", prefix, uuid);
    // Equivalent of printf("%s\n", uuid); - just my personal preference
    log_info("id: %s", ret_str);
    return ret_str;
}