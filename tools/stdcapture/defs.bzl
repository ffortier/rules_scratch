def stdcapture_action(ctx, *, executable, arguments, stdcapture, stdout = None, stderr = None, env = {}, **kwargs):
    args = ctx.actions.args()

    if "outputs" in kwargs.keys():
        fail("Outputs will be ignored, use stdout and stderr instead")

    outputs = []

    if stdout != None:
        outputs.append(stdout)
        args.add("--stdout").add(stdout)

    if stderr != None:
        outputs.append(stderr)
        args.add("--stderr").add(stderr)

    if len(outputs) == 0:
        fail("At least one of stdout and stderr must be specified")

    args.add("--")
    args.add(executable)
    args.add_all(arguments)

    new_env = {"BAZEL_BINDIR": "."}

    new_env.update(env)

    ctx.actions.run(
        executable = stdcapture,
        tools = [executable],
        arguments = [args],
        outputs = outputs,
        env = new_env,
        **kwargs
    )
