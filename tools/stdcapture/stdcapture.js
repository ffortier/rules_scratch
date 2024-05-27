import { spawn } from 'node:child_process';
import { createWriteStream } from 'node:fs';
import path from 'node:path';

const command = [];
let stdout;
let stderr;

parse_args: for (let i = 2; i < process.argv.length; i++) {
    const arg = process.argv[i];

    switch (arg) {
        case '--stdout': stdout = resolvePath(process.argv[++i]); break;
        case '--stderr': stderr = resolvePath(process.argv[++i]); break;
        case '--': command.push(...process.argv.slice(i + 1)); break parse_args;
    }
}

check(command.length > 0, 'Missing command');
check(!!stdout || !!stderr, 'At least one of stdout or stderr must be specified');

const proc = spawn(command[0], command.slice(1), { stdio: 'pipe' });

if (stdout) {
    proc.stdout.pipe(createWriteStream(stdout), { end: true });
}

if (stderr) {
    proc.stderr.pipe(createWriteStream(stderr), { end: true });
}

const exitCode = await new Promise((resolve, reject) => proc.once('exit', (code) => {
    if (code !== 0) {
        console.error(`Command exited with ${code}: ${command.join(' ')}`);
    }
    resolve(code);
}));

process.exit(exitCode);

function check(condition, message) {
    if (!condition) {
        printUsage()
        console.error(`Error: ${message}`);
        process.exit(1);
    }
}

function printUsage() {
    const { BUILD_WORKSPACE_DIRECTORY } = process.env;

    const program = !!BUILD_WORKSPACE_DIRECTORY
        ? 'bazel run -- @scratch_rules//tools/stdcapture'
        : `${process.argv[0]} ${process.argv[1]}`;

    console.error(`Usage: ${program} [--stdout <stdout.txt>] [--stderr <stderr.txt>] -- <command...>`);
    console.error(`Flags:`);
    console.error(`\t--stdout <stdout.txt>:\tThe schema json file`);
    console.error(`\t--stderr <stderr.txt>:\tThe output cpp file`);
    console.error(`\t<command...>:\t\tThe command to run`);
}

function resolvePath(p) {
    const { BUILD_WORKING_DIRECTORY } = process.env;

    if (BUILD_WORKING_DIRECTORY) {
        return path.resolve(BUILD_WORKING_DIRECTORY, p);
    }

    return p;
}
