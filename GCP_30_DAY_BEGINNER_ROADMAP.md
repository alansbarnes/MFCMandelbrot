# 30-Day Beginner Roadmap for GCP Architecture

## How to use this roadmap
- Plan for 1-2 hours per day.
- Spend about 60% of your time learning concepts and 40% doing hands-on work.
- Keep one running set of notes with: what each service does, when to use it, key tradeoffs, and any cost or security concerns.
- Use the GCP free tier where possible and set billing alerts on day 1.

## Days 1-5: Cloud foundations

### Day 1: Cloud basics and account setup
**Learn**
- Cloud service models: IaaS, PaaS, serverless
- Regions, zones, global services
- Shared responsibility model
- Pay-as-you-go pricing

**Hands-on**
- Create a GCP account and one learning project
- Open the Google Cloud Console and Cloud Shell
- Set a billing budget and alert
- Explore the dashboard, project selector, and APIs & Services page

**Focus services**
- Cloud Console
- Cloud Shell
- Billing
- Resource Manager

**Free resources**
- Google Cloud documentation: product overview pages
- Google Cloud Skills Boost introductory labs

### Day 2: Core GCP services overview
**Learn**
- What Compute Engine, Cloud Storage, VPC, Cloud SQL, GKE, Cloud Run, and BigQuery are for
- Managed service vs self-managed tradeoffs

**Hands-on**
- Visit each product page in the console
- Write one sentence for when you would use each service
- Create one Cloud Storage bucket in a valid region and then delete it after review if you want to avoid drift

**Checkpoint**
- You should be able to explain the difference between compute, storage, networking, and analytics services

### Day 3: Networking fundamentals
**Learn**
- IP addressing, subnets, routing, DNS
- Firewalls and load balancers
- Public vs private access

**Hands-on**
- Inspect the default VPC network
- Review subnet regions and firewall rules
- Find the available load balancer types in GCP

**Focus services**
- VPC
- Cloud Load Balancing
- Cloud DNS
- Firewall rules

### Day 4: Compute choices
**Learn**
- VMs vs containers vs serverless
- Stateless workloads and autoscaling
- When to choose Compute Engine, Cloud Run, App Engine, or GKE

**Hands-on**
- Compare service pages and pricing models
- Create a decision table: workload type, operational overhead, scaling pattern, best-fit service

### Day 5: Week 1 review
**Review questions**
- What is the difference between a region and a zone?
- Which GCP services are global?
- When would you choose Cloud Run over Compute Engine?
- What does the shared responsibility model mean in practice?

**Output**
- A one-page summary of core GCP services and what each is for

## Days 6-10: Identity, networking, and security

### Day 6: GCP resource hierarchy and IAM basics
**Learn**
- Organization, folders, projects, resources
- IAM principals, roles, and policies
- Basic roles vs predefined roles vs custom roles

**Hands-on**
- Review IAM for your project
- Identify owners, editors, viewers, and service accounts
- Write examples of least-privilege access for developer, admin, and application identities

### Day 7: Service accounts and application identity
**Learn**
- Service accounts vs user accounts
- Keyless auth preference
- Risks of over-privileged service accounts

**Hands-on**
- Create a test service account
- Assign one narrow role
- Review how workloads authenticate to GCP services

**Focus services**
- IAM
- Service Accounts
- Secret Manager

### Day 8: Secure network design
**Learn**
- Public vs private subnets and endpoints
- NAT, peering, and private communication patterns
- Ingress and egress controls

**Hands-on**
- Sketch a secure basic web app network
- Show internet-facing load balancing, private app access, and restricted database access

### Day 9: Security controls and observability basics
**Learn**
- Encryption at rest and in transit
- Secret storage
- Audit logs and operational logs
- Basic monitoring and alerting

**Hands-on**
- Open Cloud Logging and review available logs
- Find Cloud Audit Logs
- Review Secret Manager and Security Command Center product capabilities

**Focus services**
- Cloud Logging
- Cloud Monitoring
- Secret Manager
- Security Command Center

### Day 10: Week 2 review
**Output**
- A secure project design checklist covering IAM, network boundaries, secrets, logging, and auditability

**Checkpoint**
- You should be able to sketch a secure baseline GCP environment and explain least privilege

## Days 11-15: Compute and application hosting

### Day 11: Compute Engine fundamentals
**Learn**
- VM instances, machine types, disks, images
- Instance groups and autoscaling concepts
- When VM control is worth the added operations burden

**Hands-on**
- Walk through the instance creation wizard without necessarily launching a paid resource
- Review startup scripts, metadata, and firewall options

### Day 12: Cloud Run fundamentals
**Learn**
- Containerized serverless workloads
- Request-based scaling
- Stateless service design
- Revisions and traffic splitting

**Hands-on**
- Use a sample container image or hello world sample
- Deploy a simple app to Cloud Run
- Test the public endpoint

### Day 13: App Engine and GKE overview
**Learn**
- App Engine standard use cases
- Why teams choose GKE
- Tradeoffs between Cloud Run and GKE

**Hands-on**
- Write a comparison chart across Compute Engine, App Engine, Cloud Run, and GKE using these columns: control, scaling, complexity, best use case

### Day 14: Reliability and deployment patterns
**Learn**
- Health checks
- Rolling deployments
- Blue/green and canary basics
- Stateless app design and externalized state

**Hands-on**
- Review Cloud Run revision rollout behavior
- Map where health checks and autoscaling policies fit in a simple architecture

### Day 15: Week 3 review
**Output**
- A compute selection guide for beginner workloads
- One simple deployed service in Cloud Run or a written walkthrough if you cannot deploy

## Days 16-20: Data and storage architecture

### Day 16: Storage fundamentals
**Learn**
- Object, block, and file storage
- Durability vs performance patterns
- Backup and recovery basics

**Hands-on**
- Compare Cloud Storage, Persistent Disk, and Filestore
- Document example use cases for each

### Day 17: Operational databases
**Learn**
- Relational vs document databases
- Managed database tradeoffs
- Transactions, schema, scaling, and backups

**Hands-on**
- Compare Cloud SQL, Firestore, and Spanner
- Write a short rule of thumb for when each is appropriate

### Day 18: Analytics and reporting
**Learn**
- Operational workloads vs analytical workloads
- Data warehouses and columnar analytics

**Hands-on**
- Review BigQuery basics
- Understand datasets, tables, and query pricing at a high level
- Map a reporting pipeline from app data to analytics

### Day 19: Caching and performance
**Learn**
- Caching layers and common patterns
- Where Memorystore fits
- Read-heavy vs write-heavy considerations

**Hands-on**
- Add cache placement to one of your earlier architecture sketches
- Note invalidation and failure-mode tradeoffs

### Day 20: Week 4 review
**Output**
- A data service selection matrix for Cloud Storage, Persistent Disk, Filestore, Cloud SQL, Firestore, Spanner, BigQuery, and Memorystore

## Days 21-25: Architecture patterns and operations

### Day 21: 3-tier web architecture
**Learn**
- Presentation, application, and data tiers
- Load balancing, stateless services, and database placement

**Hands-on**
- Draw a 3-tier GCP architecture using Cloud Load Balancing, Cloud Run or Compute Engine, and Cloud SQL

### Day 22: Event-driven architecture
**Learn**
- Pub/Sub basics
- Asynchronous workflows
- Loose coupling and retry patterns

**Hands-on**
- Design a system where uploads or orders trigger downstream processing using Pub/Sub and Cloud Run or Cloud Functions

### Day 23: Operations and observability
**Learn**
- Metrics, logs, traces, alerts, dashboards
- Error budgets and SLOs at a beginner level

**Hands-on**
- Define what you would monitor for latency, errors, traffic, saturation, and cost in one sample app

### Day 24: Reliability and disaster recovery
**Learn**
- High availability, fault tolerance, multi-zone design
- RTO and RPO basics
- Backup, restore, and regional failure thinking

**Hands-on**
- Update one architecture with HA choices and disaster recovery notes

### Day 25: Cost and service tradeoffs
**Learn**
- Fixed vs variable cost patterns
- Managed service premiums vs ops savings
- Rightsizing and scaling efficiency

**Hands-on**
- Review GCP pricing calculators or pricing pages
- Compare rough cost drivers for Compute Engine, Cloud Run, and GKE

**Output**
- A short tradeoff summary for cost, scale, reliability, and operational overhead

## Days 26-30: Mini architecture portfolio

### Day 26: Static website architecture
**Build or design**
- Cloud Storage for static hosting assets
- Cloud CDN for caching
- Cloud DNS for domain routing
- HTTPS load balancing if appropriate for your design

**Document**
- Security controls
- Cost profile
- Scaling behavior

### Day 27: Web application architecture
**Build or design**
- Cloud Run for the app tier
- Cloud SQL for the database
- Load balancing in front of the app
- Secret Manager for configuration secrets

**Document**
- Network boundaries
- App-to-database access
- Backup and availability choices

### Day 28: Event-driven workflow architecture
**Build or design**
- Pub/Sub for messaging
- Cloud Run or Cloud Functions for processing
- Cloud Storage or BigQuery depending on output type

**Document**
- Retry handling
- Idempotency concerns
- Monitoring and alerting

### Day 29: Architecture review day
**Review**
- Compare your three designs with Google reference architectures
- Identify what you would change for higher scale, tighter security, or lower cost
- Check whether you can clearly justify every service choice

### Day 30: Final summary and self-test
**Produce**
- A final summary of what each core service is for
- A one-page secure baseline architecture
- A compute decision guide
- A storage/data decision guide
- Three architecture diagrams with tradeoff notes

**Self-test questions**
- When should you choose Cloud Run instead of GKE?
- When is BigQuery a better fit than Cloud SQL?
- How do IAM and service accounts shape secure design?
- What changes when you need higher availability across zones or regions?
- Which parts of your design most affect cost?

## Recommended free resources
- Google Cloud documentation product overviews and architecture center
- Google Cloud Skills Boost free labs and learning paths
- Google Cloud YouTube architecture and getting-started content
- Google Cloud Skills Boost hands-on labs for Cloud Run, IAM, VPC, and BigQuery

## What you should know by the end
By day 30, you should be able to:
- Explain the main GCP services and where they fit
- Design a basic secure project and network layout
- Choose between VMs, containers, and serverless for simple workloads
- Choose the right data and storage services for common use cases
- Explain the main cost, scale, reliability, and operational tradeoffs in a beginner-friendly architecture
