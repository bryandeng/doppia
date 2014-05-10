#include "BaseApplication.hpp"


// import bug fixed version file
#include "../libs/boost/gil/color_base_algorithm.hpp"
#include "../libs/boost/gil/pixel.hpp"

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

#include <boost/filesystem.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>

#include "BaseApplication.hpp"

#include "helpers/Log.hpp"
#include "helpers/get_option_value.hpp"
#include "helpers/any_to_string.hpp"
#include "helpers/for_each.hpp"

#include <omp.h>

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>


namespace
{

std::ostream & log_info()
{
    return  logging::log(logging::InfoMessage, "BaseApplication");
}

std::ostream & log_debug()
{
    return  logging::log(logging::DebugMessage, "BaseApplication");
}

std::ostream & log_error()
{
    return  logging::log(logging::ErrorMessage, "BaseApplication");
}

} // end of anonymous namespace


namespace doppia
{

using logging::log;
using namespace std;

//  ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-

BaseApplication::BaseApplication()
{

    using namespace boost::posix_time;
    const ptime current_time(second_clock::local_time());

    recording_path = boost::str( boost::format("%i_%02i_%02i_%i_recordings")
                                 % current_time.date().year()
                                 % current_time.date().month().as_number()
                                 % current_time.date().day()
                                 % current_time.time_of_day().total_seconds() );

    if(boost::filesystem::exists(recording_path) == true)
    {
        // this should never happen
        printf("images_recording_path == %s\n", recording_path.string().c_str());
        throw std::runtime_error("Image recording path already exists. Please wait one second and try again.");
    }

    return;
}


BaseApplication::~BaseApplication()
{

    // nothing to do here

    return;
}


int BaseApplication::main(int argc, char *argv[])
{    
    cout << get_application_title() << endl;

    program_options::variables_map options_values;
    const bool correctly_parsed = parse_arguments(argc, argv, options_values);

    int return_value = EXIT_SUCCESS;
    if(correctly_parsed)
    {
        this->options = options_values; //store the options for future use
        setup_logging(this->log_file, options_values);

        setup_problem(options_values);
        init_gui(options_values);

        log_debug() << "Entering into main_loop" << std::endl;
        // wait until the window is close
        main_loop();
    }
    else
    {
        return_value = EXIT_FAILURE;
    }

    if(exists(recording_path))
    {
        cout << "Results were saved inside " << recording_path << endl;
    }
    cout << "End of game, have a nice day." << endl;
    return return_value;
}

program_options::options_description BaseApplication::get_args_options(const std::string application_name)
{
    program_options::options_description desc("BaseApplication options");
    desc.add_options()

    ("configuration_file,c", program_options::value<string>(), "indicate the filename of the configuration .ini file")

    ("log", program_options::value<string>()->default_value(application_name + ".out.txt"),
     "where should the data log be recorded.\n" \
     "if 'stdout' is indicated, all the messages will be written to the console\n" \
     "if 'none' is indicate, no message will be shown nor recorded")

    ;

    return desc;
}

void BaseApplication::add_args_options(program_options::options_description &desc, const std::string application_name)
{

    // add the options --
    typedef std::vector< boost::shared_ptr<boost::program_options::option_description> > base_options_t;

    // needs to be static to avoid crash when using options latter
    static program_options::options_description base_args_options = BaseApplication::get_args_options(application_name);
    // since static application_name is considered only during the first function call
    // not expected to be called twice anyway

    const base_options_t &base_options = base_args_options.options();

    for(base_options_t::const_iterator it = base_options.begin(); it != base_options.end(); ++it)
    {
        desc.add(*it);
    }

    return;
}


bool BaseApplication::parse_arguments(int argc, char *argv[], program_options::variables_map &options)
{
    const bool print_argv = false;
    if (print_argv)
    {
        for (int i=0; i<argc; i+=1)
        {
            printf("argv[%i] == %s\n",i, argv[i]);
        }
    }

    // return values
    const bool arguments_correctly_parsed = true;
    const bool arguments_not_correctly_parsed = !arguments_correctly_parsed;

    program_options::options_description desc("Allowed options");
    desc.add_options()("help", "produces this help message");

    get_options_description(desc);

    try
    {
        program_options::command_line_parser parser(argc, argv);
        parser.options(desc);

        const program_options::parsed_options the_parsed_options( parser.run() );

        program_options::store(the_parsed_options, options);
        //program_options::store(program_options::parse_command_line(argc, argv, desc), options);
        program_options::notify(options);
    }
    catch (std::exception & e)
    {
        cout << "\033[1;31mError parsing the command line options:\033[0m " << e.what () << endl << endl;
        cout << desc << endl;
        throw std::runtime_error("end of game");
        return arguments_not_correctly_parsed;
    }


    if (options.count("help"))
    {
        cout << desc << endl;
        exit(EXIT_SUCCESS);
        return arguments_correctly_parsed;
    }


    // parse the configuration file
    {

        string configuration_filename;

        if(options.count("configuration_file") > 0)
        {
            configuration_filename = get_option_value<std::string>(options, "configuration_file");
        }
        else
        {
            cout << "No configuration file provided. Using command line options only." << std::endl;
        }

        if (configuration_filename.empty() == false)
        {
            boost::filesystem::path configuration_file_path(configuration_filename);
            if(boost::filesystem::exists(configuration_file_path) == false)
            {
                cout << "\033[1;31mCould not find the configuration file:\033[0m "
                     << configuration_file_path << endl;
                return arguments_correctly_parsed;
            }

            try
            {
                fstream configuration_file;
                configuration_file.open(configuration_filename.c_str(), fstream::in);
                program_options::store(program_options::parse_config_file(configuration_file, desc), options);
                configuration_file.close();
            }
            catch (...)
            {
                cout << "\033[1;31mError parsing the configuration file named:\033[0m "
                     << configuration_filename << endl;
                cout << desc << endl;
                throw;
                return arguments_not_correctly_parsed;
            }

            cout << "Parsed the configuration file " << configuration_filename << std::endl;
        }
    }

    return arguments_correctly_parsed;
}



/// helper method called by setup_problem
void BaseApplication::setup_logging(std::ofstream &log_file, const program_options::variables_map &options)
{


    if(log_file.is_open())
    {
        // the logging is already setup
        return;
    }

    logging::get_log().clear(); // we reset previously existing options

    logging::LogRuleSet rules_for_stdout;
    rules_for_stdout.add_rule(logging::EveryMessage, "console");
    rules_for_stdout.add_rule(logging::InfoMessage, "BaseApplication");
    rules_for_stdout.add_rule(logging::InfoMessage, "SceneSettings");
    rules_for_stdout.add_rule(logging::InfoMessage, "graphics");
    rules_for_stdout.add_rule(logging::InfoMessage, "detector");
    logging::get_log().set_console_stream(std::cout, rules_for_stdout);

    const string log_option_value = get_option_value<string>(options, "log");
    // should we also have a log_level option ?

    if(log_option_value != "none")
    {
        logging::LogRuleSet logging_rules;
        logging_rules.add_rule(logging::EveryMessage, "*");

        if(log_option_value == "stdout")
        {
            logging::get_log().add(std::cout, logging_rules);
        }
        else
        {
            // log_option_value != "stdout" and log_option_value != "none"
            if(boost::filesystem::exists(log_option_value))
            {
                printf("Overwriting existing log file %s\n", log_option_value.c_str());
            }
            else
            {
                printf("Creating new log file %s\n", log_option_value.c_str());
            }

            log_file.open(log_option_value.c_str());
            assert(log_file.is_open());

            logging::get_log().add(log_file, logging_rules);
        }
    }
    else
    {
        // log_option_value == "none"
        // nothing else to do, by default logging::log() omits the messages

    }


    return;
}



void BaseApplication::init_gui(const program_options::variables_map &options)
{
    graphic_user_interface_p.reset(create_gui(options));

    return;
}


bool BaseApplication::update_gui()
{
    if(graphic_user_interface_p)
    {
        return graphic_user_interface_p->update();
    }
    else
    {
        // true means "should continue"
        return true;
    }
}

void BaseApplication::save_solution()
{

    // nothing to do here
    return;
}

void  BaseApplication::create_recording_path() const
{

    if(exists(recording_path) == false)
    {
        // create the directory
        create_directory(recording_path);
        log_info() << boost::str(boost::format("Created recordings directory %s")
                                 % recording_path.string()) << std::endl;
        record_program_options();
    }
    return;
}

void BaseApplication::record_program_options() const
{

    const string fout_filename = (recording_path / "program_options.txt").string();
    ofstream fout(fout_filename.c_str());
    program_options::variables_map::const_iterator options_it;
    for_each(options_it, options)
    {
        const program_options::variables_map::value_type &option = *options_it;
        fout << option.first << " = " << any_to_string(option.second.value());

        if(option.second.defaulted())
        {
            fout << " (default value)";
        }

        fout << std::endl;
    }

    printf("Created %s\n", fout_filename.c_str());

    return;
}



const boost::filesystem::path &BaseApplication::get_recording_path() const
{
    create_recording_path();
    return recording_path;
}





} // end of namespace doppia

//  ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
