# 30-Day Beginner Roadmap for GCP Architecture (LinkedIn Premium Version)

## How to use this roadmap
- Plan for 1-2 hours per day.
- Spend about 40% of your time on LinkedIn Learning, 40% on GCP hands-on work, and 20% on Google Cloud documentation and reference architectures.
- Use LinkedIn Learning for structured explanations, vocabulary, and architecture patterns.
- Use Google Cloud directly for product discovery, console navigation, labs, and service-specific practice.
- Keep one running set of notes with: what each service does, when to use it, key tradeoffs, and any cost or security concerns.
- Use the GCP free tier where possible and set billing alerts on day 1.

## What to learn on LinkedIn vs directly in GCP

### Best topics for LinkedIn Learning
- Cloud fundamentals and terminology
- Networking basics and security concepts
- Architecture patterns and design tradeoffs
- Containers, Kubernetes, serverless, and systems design at a conceptual level
- Reliability, observability, and cost optimization frameworks

### Best topics to learn directly in GCP
- Console navigation and project setup
- IAM configuration and service accounts
- VPCs, firewall rules, logging, and monitoring views
- Deploying on Cloud Run and exploring service settings
- Storage, database, BigQuery, and Pub/Sub product-specific workflows
- Reading reference architectures and product documentation for current GCP guidance

## Days 1-5: Cloud foundations

### Course types to prioritize on LinkedIn Learning
- Beginner cloud computing fundamentals
- Introductory networking for cloud learners
- Cloud architecture foundations

### Learn directly in GCP
- Account and billing setup
- Console navigation
- Cloud Shell basics
- Product overviews for Compute Engine, Cloud Storage, VPC, Cloud SQL, GKE, Cloud Run, and BigQuery

### Daily checklist
- [ ] Day 1: Create your GCP account, set a billing budget alert, and finish the first module of a beginner cloud fundamentals course on LinkedIn Learning.
- [ ] Day 2: Learn regions, zones, global services, and the shared responsibility model on LinkedIn Learning, then explore those concepts in the GCP Console.
- [ ] Day 3: Review networking basics on LinkedIn Learning, then inspect the default VPC, subnets, and firewall rules in GCP.
- [ ] Day 4: Watch cloud service model and compute-option lessons on LinkedIn Learning, then compare Compute Engine, Cloud Run, App Engine, and GKE in GCP.
- [ ] Day 5: Write a one-page summary of the main GCP services and when to use each one.

## Days 6-10: Identity, networking, and security

### Course types to prioritize on LinkedIn Learning
- Cloud security fundamentals
- Identity and access management basics
- Secure network design for cloud platforms

### Learn directly in GCP
- Project IAM page and role assignments
- Service accounts
- Cloud Logging, Audit Logs, Secret Manager, and Security Command Center product views
- VPC design options, firewall rules, and network boundaries

### Daily checklist
- [ ] Day 6: Learn GCP resource hierarchy and IAM concepts on LinkedIn Learning, then review your project IAM settings in GCP.
- [ ] Day 7: Study least privilege and service account concepts on LinkedIn Learning, then create a narrow-permission test service account in GCP.
- [ ] Day 8: Learn public vs private networking, NAT, and peering on LinkedIn Learning, then sketch a secure GCP web app network.
- [ ] Day 9: Review encryption, secrets, logging, and auditability on LinkedIn Learning, then inspect Cloud Logging, Audit Logs, and Secret Manager in GCP.
- [ ] Day 10: Produce a secure baseline checklist for IAM, secrets, network boundaries, and logging.

## Days 11-15: Compute and application hosting

### Course types to prioritize on LinkedIn Learning
- Serverless architecture fundamentals
- Containers and Kubernetes for beginners
- Application hosting and deployment strategies

### Learn directly in GCP
- Compute Engine instance configuration flows
- Cloud Run deployments and revisions
- App Engine and GKE product capabilities
- Health checks, autoscaling, and rollout settings

### Daily checklist
- [ ] Day 11: Learn VM concepts on LinkedIn Learning, then walk through the Compute Engine instance creation flow in GCP.
- [ ] Day 12: Study serverless and stateless application design on LinkedIn Learning, then deploy a simple app to Cloud Run.
- [ ] Day 13: Learn container orchestration and Kubernetes basics on LinkedIn Learning, then compare Cloud Run, App Engine, and GKE in GCP.
- [ ] Day 14: Review autoscaling, health checks, and deployment patterns on LinkedIn Learning, then map those concepts to Cloud Run or Compute Engine settings.
- [ ] Day 15: Write a compute decision guide for simple beginner workloads.

## Days 16-20: Data and storage architecture

### Course types to prioritize on LinkedIn Learning
- Data architecture fundamentals
- Relational vs NoSQL databases
- Data analytics and caching basics

### Learn directly in GCP
- Cloud Storage, Persistent Disk, and Filestore product differences
- Cloud SQL, Firestore, Spanner, BigQuery, and Memorystore overviews
- GCP backup, availability, and pricing concepts for storage and data services

### Daily checklist
- [ ] Day 16: Learn object, block, and file storage concepts on LinkedIn Learning, then compare Cloud Storage, Persistent Disk, and Filestore in GCP.
- [ ] Day 17: Review relational vs document database tradeoffs on LinkedIn Learning, then compare Cloud SQL, Firestore, and Spanner in GCP.
- [ ] Day 18: Learn analytics workload patterns on LinkedIn Learning, then review BigQuery datasets, tables, and pricing concepts in GCP.
- [ ] Day 19: Study caching fundamentals on LinkedIn Learning, then place Memorystore into one of your architecture sketches.
- [ ] Day 20: Build a data-service selection matrix for storage, databases, analytics, and caching.

## Days 21-25: Architecture patterns and operations

### Course types to prioritize on LinkedIn Learning
- Systems design fundamentals
- Event-driven architecture
- Site reliability, observability, and cost optimization

### Learn directly in GCP
- Pub/Sub patterns and product capabilities
- Cloud Monitoring and Cloud Logging workflows
- GCP pricing pages and architecture center references
- Service-specific reliability and multi-zone guidance

### Daily checklist
- [ ] Day 21: Learn 3-tier architecture concepts on LinkedIn Learning, then draw a GCP 3-tier web architecture.
- [ ] Day 22: Study event-driven design and asynchronous messaging on LinkedIn Learning, then sketch a Pub/Sub-based workflow in GCP.
- [ ] Day 23: Learn observability concepts on LinkedIn Learning, then define metrics, logs, and alerts for a sample GCP application.
- [ ] Day 24: Review high availability, RTO, and RPO on LinkedIn Learning, then update one architecture with multi-zone and recovery choices.
- [ ] Day 25: Study cloud cost optimization on LinkedIn Learning, then compare major cost drivers for Compute Engine, Cloud Run, and GKE.

## Days 26-30: Mini architecture portfolio

### Course types to prioritize on LinkedIn Learning
- Architecture case studies
- Cloud security review and design tradeoffs
- Technical communication or diagramming for architects

### Learn directly in GCP
- Architecture Center reference patterns
- Service placement and connectivity decisions
- Product-specific security and scaling controls
- Console and documentation checks for each chosen service

### Daily checklist
- [ ] Day 26: Design a static website architecture with Cloud Storage as the origin, an external Application Load Balancer with Cloud CDN, and Cloud DNS.
- [ ] Day 27: Design a web app architecture using Cloud Run, Cloud SQL, load balancing, and Secret Manager.
- [ ] Day 28: Design an event-driven workflow using Pub/Sub plus Cloud Run and define retry and monitoring behavior.
- [ ] Day 29: Compare your three designs with Google reference architectures and note what you would change for scale, security, and cost.
- [ ] Day 30: Produce your final package: service summary, secure baseline design, compute guide, data guide, and three architecture diagrams with tradeoff notes.

## Weekly study rhythm tuned for LinkedIn Premium
- Start each day with 20-30 minutes of LinkedIn Learning to build conceptual understanding.
- Spend 30-45 minutes in GCP Console, Skills Boost labs, or product documentation.
- End with 10-15 minutes of notes: what you learned, when to use each service, and what tradeoffs matter.
- At the end of each 5-day block, create one small artifact: a summary sheet, checklist, decision table, or architecture diagram.

## Suggested course-selection strategy on LinkedIn Learning
- Pick short beginner courses instead of one very long course whenever possible.
- Prioritize courses updated recently, especially for containers, Kubernetes, and serverless topics.
- Use LinkedIn Learning for foundational cloud, networking, IAM, security, systems design, observability, and cost topics.
- Avoid relying on LinkedIn Learning alone for detailed GCP product setup because product screens and exact steps change over time.

## Recommended supporting resources
- LinkedIn Learning cloud fundamentals, networking, cloud security, systems design, serverless, Kubernetes, and observability courses
- Google Cloud documentation product overviews and Architecture Center
- Google Cloud Skills Boost hands-on labs for Cloud Run, IAM, VPC, and BigQuery
- Google Cloud YouTube architecture and getting-started content

## What you should know by the end
By day 30, you should be able to:
- Explain the main GCP services and where they fit
- Design a basic secure project and network layout
- Choose between VMs, containers, and serverless for simple workloads
- Choose the right data and storage services for common use cases
- Explain the main cost, scale, reliability, and operational tradeoffs in a beginner-friendly architecture
