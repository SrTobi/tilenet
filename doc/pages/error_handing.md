Error handling			{#error_handling}
--------------

Every function of tilenet returns a code. If the function succeeds this return code will be TNOK.
Otherwise it will be the code of the error. There are two types of errors: errors and fails.

* An error indicates, that something went wrong, was incorrect or not possible
* A fail indicates, that an operation was aborted, because of the user (e.g. timeout)

The values of error codes are greater zero and the values of fail codes are lesser then zero. TNOK equals zero.

~~~~~~~~~~~~~~~~~~~~~~{.cpp}
TNERROR ec = /* tilenet function */;

if(ec == TNOK)
{
	// everything ok
}else if(ec > TNOK)
{
	// Error
}else if(ec < TNOK)
{
	// Fail
}else{
	// No... ec can't be NaN... strange :)
}
~~~~~~~~~~~~~~~~~~~~~~

For further error codes see @ref ERRORS

Error information			{#error_infos}
=================

If an error occurs the function, causing the error, will set some informations about the error into an internal error state.
This error state is thread local, so each thread has its own state. Some functions do not modify this state, because they operating on the state:

* [tilenet_get_last_error](@ref tilenet_get_last_error)
* [tilenet_get_error_string](@ref tilenet_get_error_string)
* [tilenet_get_error_int](@ref tilenet_get_error_int)
* [tilenet_get_info_list](@ref tilenet_get_info_list)

You can use these functions to get more informations about the error.
Note that the internal error state might be reseted by a call to another tilenet function (other than the above-mentioned).

~~~~~~~~~~~~~~~~~~~~~~{.cpp}




~~~~~~~~~~~~~~~~~~~~~~

For more information codes see @ref ERRORS

tilenet_get_last_error
======================

You can also get the last error by calling tilenet_get_last_error. Note that this function only returns the code of the last _Error_.
If a function succeeds the internal error state will not be set to TNOK!


~~~~~~~~~~~~~~~~~~~~~~{.cpp}

if(tilenet_create_server(0,0) > TNOK)
{
	// Only if an error is returned, tilenet_get_last_error will be related to the last call
	TNERROR ec = tilenet_get_last_error();
}



~~~~~~~~~~~~~~~~~~~~~~