- Consider errors as part of function's signature.

- Prefer structured errors (error codes, ...) to text errors.

  + Improve error handling code. Instead of searching through text which is error prone, we compare error code.
  + Easier to customize output to different receivers (humans with i18n, or machines which mostly care about error codes).
  + Simpler to aggregate and analyze later.
  + Code searching is also more convenient.

- All errors of a given function should be enclosed in an enum instead of being implicitly formed by a set of independent constants.

  .. code-block:: rust

      // Bad (but sometimes required)
      const ERROR_INVALID_INPUT: i32 = 1;
      const ERROR_MISSING_INPUT: i32 = 2;

      fn call() -> Result<(), i32> {}

      // Good
      enum Error {
          InvalidInput,
          MissingInput,
      }

      fn call() -> Result<(), Error> {}

- Avoid gigantic error enum that can be used everywhere.

  It makes error handling easier, as the default strategy is to handle all possible errors returned from a functions instead of reading the document extremely carefully. It'll also be faster to add new type of error because we can trace all users of a particular error enum quicker.

  Hence, we should have multiple error enums and do mapping/grouping between them. Mapping/grouping usually happens when an error from lower layers is passed to higher layers.

  Usually we group fatal errors that users cannot really do anything about without making changes outside of our system (e.g. db or io errors) into one enum variant. Other types of errors. each can be mapped to a seperate variant (e.g. missing input, invalid input).

  Nesting low level error enums into higher ones is also an option.

  .. code-block:: rust

      enum SystemError {
          IoError(io::Error), // external enum
          DbError(db::Error), // external enum
      }

      enum ApplicationError {
          InvalidInput,
          MissingInput,
      }

      enum HighLevelError {
          ApplicationError(ApplicationError),
          SystemError(SystemError),
      }

- Add fail points to conditionally trigger errors of a function during testing. This can be combined with property based testing (which generates random inputs).

  + https://docs.rs/fail/0.4.0/fail/
  + http://sled.rs/errors

- Separate expected errors and unexpected errors.

  Let's say we write a function to read configurations from a file, it usually has three outcomes (assuming that the configuration file is in good format). The first one is the configurations, second is file not found error, and third one is IO error. Often, we stop the program when we encounter the third error, but use the default configuration in second case.

  So if we use ``Result<Config, MyError>`` as the return type, the caller would look like this

    .. code-block:: rust

        let config = match get_config() {
            Ok(config) => config,
            Err(MyError::FileNotFound) => get_default_config(),
            Err(MyError::IoError) => return error to caller
        };

  Instead, we can separate the errors, using ``Result<Result<Config, FileNotFoundError>, IoError>``. It looks more cumbersome, but now we can pass the unexpected error to upper level easier.

    .. code-block:: rust

        let config = get_config()?; // pass IoError up the stack, may add some context like "reading config from file"
        let config = config.unwrap_or_else(|_| get_default_config());

- Function calls and errors flow

  .. code-block:: text

           calls      request      errors
      concrete |         1         ^ general
               |       _/ \_       |
               |     2       5     |
               |    / \     / \    |
               |   3   4   6   7   |
       general v                   | concrete

  Therefore, keeping context of errors is crucial to construct an understandable error message.
