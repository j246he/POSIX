#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
using namespace std;

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <cgi++/all>
using namespace cgipp;

/* This program (web app) posts the job as a file under a
   designated directory for the consumer processes to
   deal with it. */

int main()
{
    CGI_parameters request;
    const string & user = request["user"].substr(0,30);
    const string & ip = remote_addr();

    cout << "Content-type: text/plain\n\n";

    umask (0);
    mkdir ("/tmp/.jobrequests", 0777);

    ostringstream filename;
    filename << hex << time(NULL) << '-' << hex << getpid() << ".txt";

    const string & path = "/tmp/.jobrequests/tmp-" + filename.str();
    ofstream file (path.c_str());
    if (!file)
    {
        cout << "Could not open file " << filename.str() << endl;
        return 0;
    }

    file << current_date("yyyy-mm-dd") << ' ' << current_time("HH:MM:SS")
        << '\n' << user << ' ' << ip << endl;
    file.close();

    if (rename (path.c_str(), ("/tmp/.jobrequests/" + filename.str()).c_str()) != 0)
    {
        cout << "Failed to rename file -- request failed" << endl;
    }
    else
    {
        cout << "Request by " << user << " received from IP " << remote_addr()
             << " --- processing will be displayed on the projector" << endl;
    }

    return 0;
}
