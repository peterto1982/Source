pipeline {
    agent any
    environment {
        PROJECT_DIR='sysmon-lite'
        BUILD_DIR='build'
        ARTIFACT_DIR='artifacts'
    }
    options {
        timestamps()
        timeout(time:30, unit:'MINUTES')
    }
    stages {
        stage('Checkout'){steps{checkout scm}}
        stage('System Information'){
            steps{
                sh '''
                hostname
                uname -a
                g++ --version
                cmake --version
                '''
            }
        }
        stage('Configure'){
            steps{
                dir(env.PROJECT_DIR){
                    sh '''
                    mkdir -p ${BUILD_DIR}
                    cd ${BUILD_DIR}
                    cmake ..
                    '''
                }
            }
        }
        stage('Build'){
            steps{
                dir(env.PROJECT_DIR){
                    sh '''
                    cd ${BUILD_DIR}
                    make -j$(nproc)
                    '''
                }
            }
        }
        stage('Quality Checks'){
            parallel{
                stage('Cppcheck'){
                    steps{
                        dir(env.PROJECT_DIR){
                            sh 'cppcheck --enable=all src include || true'
                        }
                    }
                }
                stage('Clang Format'){
                    steps{
                        dir(env.PROJECT_DIR){
                            sh 'find src include -type f | xargs clang-format --dry-run --Werror || true'
                        }
                    }
                }
            }
        }
        stage('Run'){
            steps{
                dir(env.PROJECT_DIR){
                    sh './build/sysmon'
                }
            }
        }
        stage('Package'){
            steps{
                dir(env.PROJECT_DIR){
                    sh '''
                    mkdir -p ${ARTIFACT_DIR}
                    tar czf ${ARTIFACT_DIR}/sysmon-${BUILD_NUMBER}.tar.gz build/sysmon
                    '''
                }
            }
        }
        stage('Archive'){
            steps{
                archiveArtifacts artifacts:'sysmon-lite/artifacts/*.tar.gz', fingerprint:true
            }
        }
    }
    post{
        always{cleanWs()}
        success{echo 'Pipeline completed successfully.'}
        failure{echo 'Pipeline failed.'}
    }
}
