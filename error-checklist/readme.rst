- Prefer structured error (error codes, ...) to text error. It will be easier to parse, generate custom error format to different users (human users with i18n, or machines which mostly care about error codes).

- Error codes should be represented by an enum containing all possible errors.

- Don't create a gigantic error enum and use it everywhere. Instead, we should have multiple error enums and do mapping/grouping between them. Mapping/grouping usually happens when an error from lower layers is passed to higher layers.

  Usually we can group fatal errors that users cannot really do anything about without making changes outside of our system into one enum variant. Other types of errors. each can be mapped to a seperate variant.

- Nesting errors is also an option.

  .. code-block:: text

      enum SystemError {
          IoError(io::Error), // external enum
          DbError(db::Error), // external enum
      }

      enum ApplicationError {
          InvalidInput,
      }

      enum HighLevelError {- Prefer structured error (error codes, ...) to text error. It will be easier to parse, generate custom error format to different users (human users with i18n, or machines which mostly care about error codes)

- Adding fail points to conditionally trigger errors of a function during testing. This can be combined with property based testing (which generates random inputs).

  + https://docs.rs/fail/0.4.0/fail/
  + http://sled.rs/errors

- Separate expected errors and unexpected errors. Let's say we write a function to read configurations from a file, it usually has three outcomes (assuming that the configuration file is in good format). The first one is the configurations, second is file not found error, and third one is IO error. Often, we stop the program when we encounter the third error, but use the default configuration in second case.

  So if we use ``Result<Config, MyError>`` as the return type, the caller would look like this

    .. code-block:: text

        let config = match get_config() {
            Ok(config) => config,
            Err(MyError::FileNotFound) => get_default_config(),
            Err(MyError::IoError) => return error to caller
        };

  Instead, we can separate the errors, using ``Result<Result<Config, FileNotFoundError>, IoError>``. It looks more cumbersome, but now we can pass the unexpected error to upper level easier.

    .. code-block:: text

        let config = match get_config()?; // pass IoError up the stack
        let config = config.unwrap_or_else(|_| get_default_config());

          SystemError,
          ApplicationError,
      }
