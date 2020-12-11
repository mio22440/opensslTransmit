#include <string>
using namespace std;
class User
{
public:
    static long name_len;
    static long len;
    string work_dir;
    char *name;
    User();
    User(char *name, string work_dir);
    ~User();
};