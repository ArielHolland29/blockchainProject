import os
import subprocess
import time

# Configuration
REPO_DIR = os.getcwd() # Assumes running from the repo root
FILE_TO_MODIFY = os.path.join("src", "main.cpp")
FLAG = "CTF{Y0UFoundM3}"

def run_command(command, cwd=REPO_DIR):
    try:
        subprocess.run(command, cwd=cwd, check=True, shell=True, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        print(f"Success: {command}")
    except subprocess.CalledProcessError as e:
        print(f"Error executing {command}: {e.stderr}")
        exit(1)

def modify_file(filepath, content_modifier):
    with open(filepath, 'r') as f:
        content = f.read()
    
    new_content = content_modifier(content)
    
    with open(filepath, 'w') as f:
        f.write(new_content)

def commit(message, date_offset_minutes=0):
    # Fake date to make it look like a real history (optional, currently just sequential)
    run_command(f'git add "{FILE_TO_MODIFY}"')
    run_command(f'git commit -m "{message}"')
    time.sleep(1) # Ensure timestamps differ slightly

def main():
    print(f"Setting up CTF in {REPO_DIR}")

    # 1. Initialize Git
    if os.path.exists(os.path.join(REPO_DIR, ".git")):
        print("Git already initialized. Skipping init.")
    else:
        run_command("git init")
        run_command('git config user.email "dev@blockchain-sim.com"')
        run_command('git config user.name "SimBlockchain Dev"')
        run_command("git add .")
        run_command('git commit -m "Initial commit of blockchain simulation"')

    # 2. Filler Commit: Feat Logging
    def step2(content):
        return "#include <iostream>\n// Logger struct for debug\nstruct Logger { void log(std::string s){} };\n" + content.replace("#include <iostream>\n", "")
    modify_file(FILE_TO_MODIFY, step2)
    commit("feat: add basic logging infrastructure")

    # 3. Filler Commit: Style
    def step3(content):
        return content + "\n// Adjusted code style\n"
    modify_file(FILE_TO_MODIFY, step3)
    commit("style: fix minor indentation issues")

    # 4. Filler Commit: Docs
    def step4(content):
        return content.replace("int main(){", "int main(){ // Main entry point for simulation")
    modify_file(FILE_TO_MODIFY, step4)
    commit("docs: update function documentation")

    # 5. Filler Commit: Refactor
    def step5(content):
        return content.replace("std::cout << \"Blockchain simulation\\n\";", "std::cout << \"Starting Blockchain simulation...\\n\";")
    modify_file(FILE_TO_MODIFY, step5)
    commit("refactor: improve startup message")

    # 6. VULNERABLE COMMIT: Add Flag
    def step6(content):
        # Insert the flag
        lines = content.splitlines()
        for i, line in enumerate(lines):
            if "int main(){" in line:
                lines.insert(i+2, f'    std::string apiKey = "{FLAG}"; // TODO: Load from env')
                lines.insert(i+3, '    std::cout << "DEBUG: Cloud Sync Enabled with Key: " << apiKey << "\\n";')
                break
        return "\n".join(lines)
    modify_file(FILE_TO_MODIFY, step6)
    commit("feat: add experimental cloud sync support")

    # 7. FIX COMMIT: Remove Flag
    def step7(content):
        lines = content.splitlines()
        new_lines = []
        for line in lines:
            if FLAG in line:
                continue # Skip the line with the flag
            if "Cloud Sync Enabled" in line:
                continue # Skip the usage line
            new_lines.append(line)
        return "\n".join(new_lines)
    modify_file(FILE_TO_MODIFY, step7)
    commit("fix: remove hardcoded secrets (security)")

    # 8. Filler Commit: Chore
    def step8(content):
        return content.replace("#include <vector>", "#include <vector> // Standard vector container")
    modify_file(FILE_TO_MODIFY, step8)
    commit("chore: add comments to includes")

    # 9. Filler Commit: Test
    def step9(content):
        return content + "\n// Tests: all unit tests passed locally.\n"
    modify_file(FILE_TO_MODIFY, step9)
    commit("test: verify basic block generation")

    # 10. Filler Commit: Perf
    def step10(content):
        return content.replace("using u32 = uint32_t;", "using u32 = uint32_t; // Optimized for 32-bit systems")
    modify_file(FILE_TO_MODIFY, step10)
    commit("perf: micro-optimization for typedefs")

    # 11. Filler Commit: Docs
    def step11(content):
        return content.replace("std::cout << \"Do you want to see specific user's balances?(y/n): \";", "std::cout << \"View specific user balance? (y/n): \";")
    modify_file(FILE_TO_MODIFY, step11)
    commit("docs: clarify UI prompts")

    # 12. Filler Commit: Refactor and cleanup
    def step12(content):
        # Remove the logger at the top to clean up
        return content.replace("// Logger struct for debug\nstruct Logger { void log(std::string s){} };\n", "")
    modify_file(FILE_TO_MODIFY, step12)
    commit("refactor: cleanup unused logger struct")

    print("CTF Git History Generation Complete!")

if __name__ == "__main__":
    main()
